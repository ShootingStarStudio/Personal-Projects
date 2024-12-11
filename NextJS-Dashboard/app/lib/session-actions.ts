'use server';

import { z } from 'zod';
import { sql } from '@vercel/postgres';
import { revalidatePath } from 'next/cache';
import { redirect } from 'next/navigation';
import { randomUUID } from 'crypto';



const FormSchema = z.object({
    id: z.string(),
    sessionName: z.string({
      invalid_type_error: 'Please input Session Name.',
      }),
    sessionDate: z.string({
      invalid_type_error: 'Please input Session Date.',
      }),
    status: z.enum(['uncompleted', 'completed'], {
      invalid_type_error: 'Please select an session status.',
      }),
  });
  
  export type State = {
    errors?: {
      sessionName?: string[];
      sessionDate?: string[];
      status?: string[];
    };
    message?: string | null;
  };
  
  const CreateSession = FormSchema.omit({ id: true, date: true });
  
  export async function createSession(prevState: State, formData: FormData) {
      //validate using zod
      const validatedFields = CreateSession.safeParse({
          sessionName: formData.get('sessionName'),
          sessionDate: formData.get('sessionDate'),
          status: formData.get('status'),
      });
      //if validation fails, return errors, else continue
      if (!validatedFields.success) {
          return {
            errors: validatedFields.error.flatten().fieldErrors,
            message: 'Missing Fields. Failed to Create Session.',
          };
      }
      //prepare data for insertion into database
      const { sessionName, sessionDate, status } = validatedFields.data;
      //const sessionDate = new Date().toISOString().split('T')[0];
  
      try {
          await sql`
              INSERT INTO sessions (id, name, date, status)
              VALUES (${randomUUID()}, ${sessionName}, ${sessionDate}, ${status})
          `;
      } catch (error) {
          return {
              message: 'Database Error: Failed to Create Session.',
          };
      }
      
      revalidatePath('/dashboard/sessions');
      redirect('/dashboard/sessions');
  }
  
  const EditSession = FormSchema.omit({ id: true, date: true });
  
  export async function editSession(
      id: string, 
      prevState: State,
      formData: FormData) {
      const validatedFields = EditSession.safeParse({
        sessionName: formData.get('sessionName'),
        sessionDate: formData.get('sessionDate'),
        status: formData.get('status'),
      });
  
      if (!validatedFields.success) {
          return {
            errors: validatedFields.error.flatten().fieldErrors,
            message: 'Missing Fields. Failed to Edit Session.',
          };
      }
      
      const { sessionName, sessionDate, status } = validatedFields.data;
     
      try {
          await sql`
              UPDATE sessions
              SET name = ${sessionName}, date = ${sessionDate}, status = ${status}
              WHERE id = ${id}
          `;
      } catch (error) {
          return { message: 'Database Error: Failed to Edit Session.' };
      }
  
      revalidatePath('/dashboard/sessions');
      redirect('/dashboard/sessions');
  }
  
  export async function deleteSession(id: string) { 
      try {
          await sql`DELETE FROM sessions WHERE id = ${id}`;
          revalidatePath('/dashboard/sessions');
          return { message: 'Deleted Session.' };
      } catch (error) {
          return { message: 'Database Error: Failed to Delete Session.' };
      }
  }
  
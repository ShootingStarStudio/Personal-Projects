'use server';

import { z } from 'zod';
import { sql } from '@vercel/postgres';
import { revalidatePath } from 'next/cache';
import { redirect } from 'next/navigation';

const FormSchema = z.object({
    id: z.string(),
    name: z.string(),
  });
  
  export type State = {
    errors?: {
      id?: string[];
      name?: string[];
    };
    message?: string | null;
  };
  
  const CreateCenter = FormSchema;
  
  export async function createCenter(prevState: State, formData: FormData) {
      //validate using zod
      const validatedFields = CreateCenter.safeParse({
        id: formData.get('id'),
        name: formData.get('name'),
      });
      //if validation fails, return errors, else continue
      if (!validatedFields.success) {
          return {
            errors: validatedFields.error.flatten().fieldErrors,
            message: 'Missing Fields. Failed to Create Center.',
          };
      }
      //prepare data for insertion into database
      const { id, name} = validatedFields.data;
      try {
          await sql`
              INSERT INTO centers (id, name)
              VALUES (${id}, ${name})
          `;
      } catch (error) {
          return {
              message: 'Database Error: Failed to Create Center.',
          };
      }  console.log("creating center fields");
      revalidatePath('/dashboard/centers');
      redirect('/dashboard/centers');
  }
  
  const EditCenter = FormSchema.omit({ id: true });
  
  export async function editCenter(
      id: string, 
      prevState: State,
      formData: FormData) {
      const validatedFields = EditCenter.safeParse({
        name: formData.get('name'),
      });
  
      if (!validatedFields.success) {
          return {
            errors: validatedFields.error.flatten().fieldErrors,
            message: 'Missing Fields. Failed to Edit Center.',
          };
      }
      const {name} = validatedFields.data;
     
      try {
          await sql`
              UPDATE centers
              SET name = ${name}
              WHERE id = ${id}
          `;
      } catch (error) {
          return { message: 'Database Error: Failed to Edit Center.' };
      }
  
      revalidatePath('/dashboard/centers');
      redirect('/dashboard/centers');
  }
  
  export async function deleteCenter(id: string) { 
      try {
          await sql`DELETE FROM sessions WHERE id = ${id}`;
          revalidatePath('/dashboard/centers');
          return { message: 'Deleted Center.' };
      } catch (error) {
          return { message: 'Database Error: Failed to Delete Center.' };
      }
  }
  
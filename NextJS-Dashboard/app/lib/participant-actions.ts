'use server';

import { z } from 'zod';
import { sql } from '@vercel/postgres';
import { revalidatePath } from 'next/cache';
import { redirect } from 'next/navigation';
import { fetchParticipants } from './data';


const FormSchema = z.object({
  id: z.string(),
  name: z.string(),
  participantId: z.string({
    invalid_type_error: 'Please select a participant.',
    }),
  centerId: z.string(),
});

export type State = {
  errors?: {
    participantId?: string[];
    centerId?: string[];
  };
  message?: string | null;
};

const CreateParticipant = FormSchema.omit({id: true, participantId: true});

export async function createParticipant(prevState: State, formData: FormData) {
  //validate using zod
  const validatedFields = CreateParticipant.safeParse({
    name: formData.get('name'),
    centerId: formData.get('centerID'),
  });
  //if validation fails, return errors, else continue
  
  //Needs further testing to confirm if the +1 is necessary
  const newIDNum = (await fetchParticipants()).length + 1;
  var finalIDVar: string;
  
  if(newIDNum < 10) {
    finalIDVar = '0' + newIDNum.toString();
  } else {
    finalIDVar = newIDNum.toString();
  }
  
  if (!validatedFields.success) {
      console.log(validatedFields.error.flatten().fieldErrors);
      return {
        errors: validatedFields.error.flatten().fieldErrors,
        message: 'Missing Fields. Failed to Create Participant.',
      };
  }
  
  //prepare data for insertion into database
  //const { participantId, amount, centreID, status } = validatedFields.data;
  const { name, centerId } = validatedFields.data;
  //const amountInCents = amount * 100;         //convert to cents to prevent floating point error
  //const date = new Date().toISOString().split('T')[0];
  
  //const newIDNum = (await fetchParticipants()).length;
  //const newId = centerId+newIDNum.toString();
  
  try {
    //console.log("It was a send");
    await sql`
      INSERT INTO participants (id, name, center_id)
      VALUES (
        CONCAT(CAST(${centerId} AS VARCHAR), CAST(${finalIDVar} AS VARCHAR)), 
        ${name}, 
        ${centerId}
      )
    `;

  } catch (error) {
    console.log(error);
    return {
        message: 'Database Error: Failed to Create Participant.',
    };
  }

  revalidatePath('/dashboard/participants');
  redirect('/dashboard/participants');
}

const EditParticipant = FormSchema.omit({ id: true, participantId: true });

export async function editParticipant(
    id: string, 
    prevState: State,
    formData: FormData) {
    const validatedFields = EditParticipant.safeParse({
      name: formData.get('name'),
      centerId: formData.get('centerID'),
    });
    
    if (!validatedFields.success) {
      console.log(validatedFields.error.flatten().fieldErrors);
        return {
          errors: validatedFields.error.flatten().fieldErrors,
          message: 'Missing Fields. Failed to Edit Participant.',
        };
    }
    
    const { name, centerId } = validatedFields.data;
    
    try {
        await sql`
            UPDATE participants
            SET name = ${name}, center_id = ${centerId}
            WHERE id = ${id}
        `;
    } catch (error) {
        return { message: 'Database Error: Failed to Edit Participant.' };
    }

    revalidatePath('/dashboard/participants');
    redirect('/dashboard/participants');
}

export async function deleteParticipant(id: string) { 
    try {
        await sql`DELETE FROM participants WHERE id = ${id}`;
        revalidatePath('/dashboard/participants');
        return { message: 'Deleted Participant.' };
    } catch (error) {
        return { message: 'Database Error: Failed to Delete Participant.' };
    }
}

'use server';

import { z } from 'zod';
import { sql } from '@vercel/postgres';
import { revalidatePath } from 'next/cache';
import { redirect } from 'next/navigation';

import { randomUUID } from 'crypto';

const FormSchema = z.object({
  id: z.string(),
  name: z.string({
    invalid_type_error: 'Please Enter a Game Name.',
    }),
  type: z.enum(['fitness' , 'cognitive' , 'social'], {
    invalid_type_error: 'Please select a Game Catergory.',
    }),
  sortby: z.enum(['descending' , 'ascending'], {
    invalid_type_error: 'Please select Sort by.',
    }),
  scoretype: z.enum(['float' , 'integer' , 'time'], {
    invalid_type_error: 'Please select a Score Type.',
    }),
  formula: z.string({
    invalid_type_error: 'Please Enter a Game Formula.',
    }),
});

export type State = {
  errors?: {
    name?: string[];
    type?: string[];
    sortby?: string[];
    scoretype?: string[];
    formula?: string[];
  };
  message?: string | null;
};
  
const CreateGame = FormSchema.omit({ id: true });

export async function createGame(prevState: State, formData: FormData) {
    //validate using zod
    const validatedFields = CreateGame.safeParse({
        name: formData.get('name'),
        type: formData.get('type'),
        sortby: formData.get('sortby'),
        scoretype: formData.get('scoretype'),
        formula: formData.get('formula'),
    });
    //if validation fails, return errors, else continue
    if (!validatedFields.success) {
        return {
          errors: validatedFields.error.flatten().fieldErrors,
          message: 'Missing Fields. Failed to Create Game.',
        };
    }console.log("create game test");
    //prepare data for insertion into database
    const { name, type, sortby, scoretype, formula } = validatedFields.data;

    try {
        await sql`
          INSERT INTO games (
            id, 
            name, 
            type, 
            sortby, 
            scoretype, 
            formula
          )
          VALUES (
            ${randomUUID()}, 
            ${name}, 
            ${type}, 
            ${sortby}, 
            ${scoretype}, 
            ${formula}
          )
        `;
    } catch (error) {
        return {
            message: 'Database Error: Failed to Create Game.',
        };
    }
    
    revalidatePath('/dashboard/games');
    redirect('/dashboard/games');
}
  
const EditGame = FormSchema.omit({ id: true });
  
export async function editGame(
    id: string, 
    prevState: State,
    formData: FormData) {
    const validatedFields = EditGame.safeParse({
      name: formData.get('name'),
      type: formData.get('type'),
      sortby: formData.get('sortby'),
      scoretype: formData.get('scoretype'),
      formula: formData.get('formula'),
    });

    if (!validatedFields.success) {
        return {
          errors: validatedFields.error.flatten().fieldErrors,
          message: 'Missing Fields. Failed to Edit Game.',
        };
    }
    
    const { name, type, sortby, scoretype, formula } = validatedFields.data;
    
    try {
        await sql`
            UPDATE games
            SET
              name = ${name}, 
              type = ${type}, 
              sortby = ${sortby}, 
              scoretype = ${scoretype}, 
              formula = ${formula}
            WHERE id = ${id}
        `;
    } catch (error) {
        return { message: 'Database Error: Failed to Edit Game.' };
    }

    revalidatePath('/dashboard/games');
    redirect('/dashboard/games');
}
  
export async function deleteGame(id: string) { 
    try {
        await sql`DELETE FROM games WHERE id = ${id}`;
        revalidatePath('/dashboard/games');
        return { message: 'Deleted Game.' };
    } catch (error) {
        return { message: 'Database Error: Failed to Delete Game.' };
    }
}
  
import { EyeIcon, PencilIcon, PlusIcon, SunIcon, TrashIcon } from '@heroicons/react/24/outline';
import Link from 'next/link';
import { deleteSession } from '@/app/lib/session-actions';
import { fetchSessionById } from '@/app/lib/data';
import line from 'next-auth/providers/line';
import { promise } from 'zod';
import { notFound } from 'next/navigation';
import { Session } from '@/app/lib/definitions';

export function CreateSession() {
  return (
    <Link
      href='/dashboard/sessions/create'
      className="flex h-10 items-center rounded-lg bg-blue-600 px-4 text-sm font-medium text-white transition-colors hover:bg-blue-500 focus-visible:outline focus-visible:outline-2 focus-visible:outline-offset-2 focus-visible:outline-blue-600"
    >
      <span className="hidden md:block">NEW</span>{' '}
      <PlusIcon className="h-5 md:ml-4" />
    </Link>
  );
}

export function ViewSession({ id }: { id: string }) {
  return (
    <Link
    href={`/dashboard/sessions/${id}/view`}
      className="rounded-md border p-2 hover:bg-gray-100"
    >
      <EyeIcon className="w-5" />
    </Link>
  );
}

export function EditSession({ id }: { id: string }) {
  return (
    <Link
    href={`/dashboard/sessions/${id}/edit`}
      className="rounded-md border p-2 hover:bg-gray-100"
    >
      <PencilIcon className="w-5" />
    </Link>
  );
}

export function DeleteSession({ id }: { id: string }) {
  const deleteSessionWithId = deleteSession.bind(null, id);
  return (
    <form action={deleteSessionWithId}>
      <button className="rounded-md border p-2 hover:bg-gray-100">
        <span className="sr-only">Delete</span>
        <TrashIcon className="w-5" />
      </button>
    </form>
  );
}

//<-- TOGGLE STATUS BUTTON WIP -->//

//new func - make session status XXX toggle with ^ button css
// export async function StatusToggle({ id }: { id: string }) {
//   const [session] = await Promise.all([
//     fetchSessionById,
//   ]);

//   if (!session)
//   {
//     notFound();
//   }
//   return (
//     <><SunIcon className="w-5" />
//       <>
//       console.log({session.arguments.status});
      
//       if({session.arguments.status} == "uncompleted")
//       {

//       }
//       else if({session.arguments.status} == "completed")
//       {
        
//       }
//       </>
      
//     </>
      
//   )
// }



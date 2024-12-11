import { EyeIcon, PencilIcon, PlusIcon, TrashIcon } from '@heroicons/react/24/outline';
import Link from 'next/link';
import { deleteParticipant } from '@/app/lib/participant-actions';

export function CreateParticipant() {
  return (
    <Link
      href='/dashboard/participants/create'
      className="flex h-10 items-center rounded-lg bg-blue-600 px-4 text-sm font-medium text-white transition-colors hover:bg-blue-500 focus-visible:outline focus-visible:outline-2 focus-visible:outline-offset-2 focus-visible:outline-blue-600"
    >
      <span className="hidden md:block">NEW</span>{' '}
      <PlusIcon className="h-5 md:ml-4" />
    </Link>
  );
}

export function ViewParticipant({ id }: { id: string }) {
  return (
    <Link
    href={`/dashboard/participants/${id}/edit`}
      className="rounded-md border p-2 hover:bg-gray-100"
    >
      <EyeIcon className="w-5" />
    </Link>
  );
}

export function EditParticipant({ id }: { id: string }) {
  return (
    <Link
    href={`/dashboard/participants/${id}/edit`}
      className="rounded-md border p-2 hover:bg-gray-100"
    >
      <PencilIcon className="w-5" />
    </Link>
  );
}

export function DeleteParticipant({ id }: { id: string }) {
  const deleteSessionWithId = deleteParticipant.bind(null, id);
  return (
    <form action={deleteSessionWithId}>
      <button className="rounded-md border p-2 hover:bg-gray-100">
        <span className="sr-only">Delete</span>
        <TrashIcon className="w-5" />
      </button>
    </form>
  );
}

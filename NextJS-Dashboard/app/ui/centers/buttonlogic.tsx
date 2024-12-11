import { EyeIcon, PencilIcon, PlusIcon, TrashIcon } from '@heroicons/react/24/outline';
import Link from 'next/link';
import { deleteCenter } from '@/app/lib/center-actions';

export function CreateCenter() {
  return (
    <Link
      href='/dashboard/centers/create'
      className="flex h-10 items-center rounded-lg bg-blue-600 px-4 text-sm font-medium text-white transition-colors hover:bg-blue-500 focus-visible:outline focus-visible:outline-2 focus-visible:outline-offset-2 focus-visible:outline-blue-600"
    >
      <span className="hidden md:block">NEW</span>{' '}
      <PlusIcon className="h-5 md:ml-4" />
    </Link>
  );
}

export function ViewCenter({ id }: { id: string }) {
  return (
    <Link
    href={`/dashboard/centers/${id}/edit`}
      className="rounded-md border p-2 hover:bg-gray-100"
    >
      <EyeIcon className="w-5" />
    </Link>
  );
}

export function EditCenter({ id }: { id: string }) {
  return (
    <Link
    href={`/dashboard/centers/${id}/edit`}
      className="rounded-md border p-2 hover:bg-gray-100"
    >
      <PencilIcon className="w-5" />
    </Link>
  );
}

export function DeleteCenter({ id }: { id: string }) {
  const deleteSessionWithId = deleteCenter.bind(null, id);
  return (
    <form action={deleteSessionWithId}>
      <button className="rounded-md border p-2 hover:bg-gray-100">
        <span className="sr-only">Delete</span>
        <TrashIcon className="w-5" />
      </button>
    </form>
  );
}

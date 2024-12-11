import { ViewSession, EditSession, DeleteSession } from '@/app/ui/sessions/buttonlogic';
import SessionStatus from '@/app/ui/sessionstatus';
import { formatDateToLocal } from '@/app/lib/utils';
import { fetchFilteredSessions } from '@/app/lib/data';

export default async function SessionsTable({
  query,
  currentPage,
}: {
  query: string;
  currentPage: number;
}) {
  const sessions = await fetchFilteredSessions(query, currentPage);

  return (
    <div className="mt-6 flow-root">
      <div className="inline-block min-w-full align-middle">
        <div className="rounded-lg bg-gray-50 p-2 md:pt-0">
          <div className="md:hidden">
            {sessions?.map((session) => (
              <div
                key={session.id}
                className="mb-2 w-full rounded-md bg-white p-4"
              >
                <div className="flex items-center justify-between border-b pb-4">
                  <p>{session.name}</p>
                  {formatDateToLocal(session.date)}
                  <SessionStatus status={session.status} />
                </div>
                <div className="flex w-full items-center justify-between pt-4">
                  <div className="flex justify-end gap-2">
                    <ViewSession id={session.id} />
                    <EditSession id={session.id} />
                    <DeleteSession id={session.id} />
                  </div>
                </div>
              </div>
            ))}
          </div>
          <table className="hidden min-w-full text-gray-900 md:table">
            <thead className="rounded-lg text-left text-sm font-normal">
              <tr>
                <th scope="col" className="px-4 py-5 font-medium sm:pl-6">
                  Session Name
                </th>
                <th scope="col" className="px-3 py-5 font-medium">
                  Date
                </th>
                <th scope="col" className="px-3 py-5 font-medium">
                  Status
                </th>
                <th scope="col" className="relative py-3 pl-6 pr-3">
                  <span className="sr-only">Edit</span>
                </th>
              </tr>
            </thead>
            <tbody className="bg-white">
              {sessions?.map((session) => (
                <tr
                  key={session.id}
                  className="w-full border-b py-3 text-sm last-of-type:border-none [&:first-child>td:first-child]:rounded-tl-lg [&:first-child>td:last-child]:rounded-tr-lg [&:last-child>td:first-child]:rounded-bl-lg [&:last-child>td:last-child]:rounded-br-lg"
                >
                  <td className="whitespace-nowrap py-3 pl-6 pr-3">
                    <div className="flex items-center gap-3">
                      <p>{session.name}</p>
                    </div>
                  </td>
                  <td className="whitespace-nowrap px-3 py-3">
                    {formatDateToLocal(session.date)}
                  </td>
                  <td className="whitespace-nowrap px-3 py-3">
                    <SessionStatus status={session.status} />
                  </td>
                  <td className="whitespace-nowrap py-3 pl-6 pr-3">
                    <div className="flex justify-end gap-3">
                      <ViewSession id={session.id} />
                      <EditSession id={session.id} />
                      <DeleteSession id={session.id} />
                    </div>
                  </td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      </div>
    </div>
  );
}

import { ViewParticipant, EditParticipant, DeleteParticipant } from '../..//ui/participants/buttonlogic';
import { fetchFilteredParticipants } from '../../lib/data';
import React from 'react';

export default async function ParticipantsTable({
  query,
  currentPage,
}: {
  query: string;
  currentPage: number;
}) {
  const participants = await fetchFilteredParticipants(query, currentPage);
   
  return (
    <div className="mt-6 flow-root">
      <div className="inline-block min-w-full align-middle">
        <div className="rounded-lg bg-gray-50 p-2 md:pt-0">
          <div className="md:hidden">
            {participants?.map((participant) => (
              <div
                key={participant.id}
                className="mb-2 w-full rounded-md bg-white p-4"
              >
                <div className="flex items-center justify-between border-b pb-4">
                  <p>{participant.name}</p>
                </div>
                <br/>
                <p>{participant.id}</p>
                <div className="flex w-full items-center justify-between pt-4">
                  <div className="flex justify-end gap-2">
                    <ViewParticipant id={participant.id} />
                    <EditParticipant id={participant.id} />
                    <DeleteParticipant id={participant.id} />
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
                  ID
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
              {participants?.map((participant) => (
                <tr
                  key={participant.id}
                  className="w-full border-b py-3 text-sm last-of-type:border-none [&:first-child>td:first-child]:rounded-tl-lg [&:first-child>td:last-child]:rounded-tr-lg [&:last-child>td:first-child]:rounded-bl-lg [&:last-child>td:last-child]:rounded-br-lg"
                >
                  <td className="whitespace-nowrap py-3 pl-6 pr-3">
                    <div className="flex items-center gap-3">
                      <p>{participant.name}</p>
                    </div>
                  </td>
                  <td className="whitespace-nowrap py-3 pl-6 pr-3">
                    <div className="flex items-center gap-3">
                      <p>{participant.id}</p>
                    </div>
                  </td>
                  <td className="whitespace-nowrap py-3 pl-6 pr-3">
                    <div className="flex justify-end gap-3">
                      <ViewParticipant id={participant.id} />
                      <EditParticipant id={participant.id} />
                      <DeleteParticipant id={participant.id} />
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

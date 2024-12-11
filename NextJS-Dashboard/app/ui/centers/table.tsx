import { ViewCenter, EditCenter, DeleteCenter } from '@/app/ui/centers/buttonlogic';
import { fetchFilteredCenters } from '@/app/lib/data';

export default async function CentersTable({
  query,
  currentPage,
}: {
  query: string;
  currentPage: number;
}) {
  const centers = await fetchFilteredCenters(query, currentPage);

  return (
    <div className="mt-6 flow-root">
      <div className="inline-block min-w-full align-middle">
        <div className="rounded-lg bg-gray-50 p-2 md:pt-0">
          <div className="md:hidden">
            {centers?.map((center) => (
              <div
                key={center.id}
                className="mb-2 w-full rounded-md bg-white p-4"
              >
                <div className="flex w-full items-center justify-between pt-4">
                  <div className="flex justify-end gap-2">
                    <ViewCenter id={center.id} />
                    <EditCenter id={center.id} />
                    <DeleteCenter id={center.id} />
                  </div>
                </div>
              </div>
            ))}
          </div>
          <table className="hidden min-w-full text-gray-900 md:table">
            <thead className="rounded-lg text-left text-sm font-normal">
              <tr>
                <th scope="col" className="px-4 py-5 font-medium sm:pl-6">
                  Center ID
                </th>
                <th scope="col" className="px-3 py-5 font-medium">
                  Center Name
                </th>
                <th scope="col" className="px-3 py-5 font-medium">
                  Participant Count
                </th>
                <th scope="col" className="relative py-3 pl-6 pr-3">
                  <span className="sr-only">Edit</span>
                </th>
              </tr>
            </thead>
            <tbody className="bg-white">
              {centers?.map((center) => (
                <tr
                  key={center.id}
                  className="w-full border-b py-3 text-sm last-of-type:border-none [&:first-child>td:first-child]:rounded-tl-lg [&:first-child>td:last-child]:rounded-tr-lg [&:last-child>td:first-child]:rounded-bl-lg [&:last-child>td:last-child]:rounded-br-lg"
                >
                  <td className="whitespace-nowrap py-3 pl-6 pr-3">
                  <div className="flex items-center gap-3">
                      <p>{center.id}</p>
                    </div>
                    <div className="flex items-center gap-3">
                      <p>{center.name}</p>
                    </div>
                  </td>
                  <td className="whitespace-nowrap py-3 pl-6 pr-3">
                    <div className="flex justify-end gap-3">
                      <ViewCenter id={center.id} />
                      <EditCenter id={center.id} />
                      <DeleteCenter id={center.id} />
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

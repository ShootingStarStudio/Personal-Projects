import { fetchFilteredGames } from '@/app/lib/data';
import { Card } from './gamecard';          //body of the card itself

export default async function GamesTable({
  query,
  currentPage,
}: {
  query: string;
  currentPage: number;
}) {
  const games = await fetchFilteredGames(query, currentPage);

  return (
    <div className="mt-6 flow-root">
      {/* <div className="inline-block min-w-full align-middle">
        <div className="rounded-lg bg-gray-50 p-2 md:pt-0">
          <div className="md:hidden">
            {games?.map((game) => (
              <div
                key={game.id}
                className="mb-2 w-full rounded-md bg-white p-4"
              >
                <div className="flex items-center justify-between border-b pb-4">
                  <p>{game.name}</p>
                </div>
              </div>
            ))}
          </div>
          <table className="hidden min-w-full text-gray-900 md:table">
            <thead className="rounded-lg text-left text-sm font-normal">
              <tr>
                <th scope="col" className="px-4 py-5 font-medium sm:pl-6">
                  Game Name
                </th>
                <th scope="col" className="px-3 py-5 font-medium">
                  Type
                </th>
                <th scope="col" className="px-3 py-5 font-medium">
                  Sort by
                </th>
                <th scope="col" className="px-3 py-5 font-medium">
                  Score Type
                </th>
              </tr>
            </thead>
            <tbody className="bg-white">
              {games?.map((game) => (
                <tr
                  key={game.id}
                  className="w-full border-b py-3 text-sm last-of-type:border-none [&:first-child>td:first-child]:rounded-tl-lg [&:first-child>td:last-child]:rounded-tr-lg [&:last-child>td:first-child]:rounded-bl-lg [&:last-child>td:last-child]:rounded-br-lg"
                >
                  <td className="whitespace-nowrap py-3 pl-6 pr-3">
                    <div className="flex items-center gap-3">
                      <p>{game.name}</p>
                    </div>
                  </td>
                  <td className="whitespace-nowrap px-3 py-3">
                    {game.type}
                  </td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      </div> */}
      {games?.map((game) => (
      <Card game = {game}/>
      ))}
    </div>
  );
}

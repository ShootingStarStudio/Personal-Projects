import { Game } from '@/app/lib/definitions';
import { 
    ViewGame, 
    EditGame, 
    DeleteGame } from '@/app/ui/games/buttonlogic';
import {
    TrophyIcon,
    PuzzlePieceIcon,
    UserGroupIcon,
} from '@heroicons/react/24/outline';
import { lusitana } from '@/app/ui/fonts';

const iconMap = {
    fitness: TrophyIcon,
    cognitive: PuzzlePieceIcon,
    social: UserGroupIcon,
};

export async function Card({ game }: {
    game: Game
}) {
  const Icon = iconMap[game.type];

  return (
    <div className="grid grid-cols-5 gap-x-2 gap-y-3 ">
      <div className="rounded-lg bg-cyan-400 p-2 shadow-sm min-h-[50px]">
      <div className="flex p-4">
        {Icon ? <Icon className="h-5 w-5 text-gray-700" /> : null}
        <h3 className="ml-2 text-sm font-medium">{game.name}</h3>
      </div>
      <p
        className={`${lusitana.className}
          truncate rounded-xl bg-white px-4 py-8 text-center text-2xl`}
      >
        {game.formula}
      </p>
      <>{game.sortby}, {game.scoretype}</>
        <div className="flex w-full items-center justify-between pt-4">
          <div className="flex justify-end gap-2">
            <ViewGame id={game.id} />
            <EditGame id={game.id} />
            <DeleteGame id={game.id} />
          </div>
        </div>
        
      </div>
    </div>
    
  );
}
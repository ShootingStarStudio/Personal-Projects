'use client';

import {
  HomeIcon,
  TrophyIcon,
  RectangleGroupIcon,
  UserGroupIcon,
  UserIcon,
} from '@heroicons/react/24/outline';
import Link from 'next/link';
import { usePathname } from 'next/navigation';
import clsx from 'clsx';

// Map of links to display in the side navigation.
// Deuncompleted on the size of the application, this would be stored in a database.
const links = [
  { 
    name: 'Home', 
    href: '/dashboard', 
    icon: HomeIcon 
  },
  { 
    name: 'Games', 
    href: '/dashboard/games', 
    icon: TrophyIcon 
  },
  {
    name: 'Sessions',
    href: '/dashboard/sessions',
    icon: RectangleGroupIcon,
  },
  { 
    name: 'Centers', 
    href: '/dashboard/centers', 
    icon: UserGroupIcon 
  },
  { 
    name: 'Participants', 
    href: '/dashboard/participants', 
    icon: UserIcon 
  },
];

export default function NavLinks() {
  const pathname = usePathname();
  return (
    <>
      {links.map((link) => {
        const LinkIcon = link.icon;
        return (
          <Link
            key={link.name}
            href={link.href}
            className={clsx(
              'flex h-[48px] grow items-center justify-center gap-2 rounded-md bg-purple-500 p-3 text-sm font-medium hover:bg-sky-100 hover:text-slate-900 md:flex-none md:justify-start md:p-2 md:px-3',
              { 'bg-sky-100 text-cyan-300': pathname === link.href, }, 
              )} 
          >
            <LinkIcon className="w-6" />
            <p className="hidden md:block">{link.name}</p>
          </Link>
        );
      })}
    </>
  );
}
import Link from 'next/link';
import NavLinks from '@/app/ui/dashboard/nav-links';
import AriseLogo from '@/app/ui/arise-logo';
import { PowerIcon, UserIcon } from '@heroicons/react/24/outline';
import { signOut } from '@/auth';
import ThemeSwitcher from './themeswitcher';

export default function SideNav() {
  return (
    <div className="flex h-full flex-col px-3 py-4 md:px-2">
      {/* logo and home button */}
      <Link
        className="mb-2 flex h-20 items-end justify-start rounded-md bg-purple-700 p-4 md:h-40"
        href="/"
      >
        <div className="w-32 text-white md:w-40">
          <AriseLogo />
        </div>
      </Link>
      {/* Navigation Links */}
      <div className="flex grow flex-row justify-between space-x-2 md:flex-col md:space-x-0 md:space-y-2">
        <NavLinks />
        <div className="hidden h-auto w-full grow rounded-md bg-purple-500 md:block"></div>        {/* spacer */}
        {/* accessibility */}
        <ThemeSwitcher />
        <div>
          <button className="flex h-[48px] w-full grow items-center justify-center gap-2 rounded-md bg-purple-500 p-3 text-sm font-medium hover:bg-sky-100 hover:text-slate-900 md:flex-none md:justify-start md:p-2 md:px-3">
            <UserIcon className="w-6" />
            <div className="hidden md:block">Profile</div>
            {/* requires form as well to handle user items */}
          </button>
        </div>
        <form action={async () => {
            'use server';
            await signOut();
          }} >
          <button className="flex h-[48px] w-full grow items-center justify-center gap-2 rounded-md bg-purple-500 p-3 text-sm font-medium hover:bg-sky-100 hover:text-slate-900 md:flex-none md:justify-start md:p-2 md:px-3">
            <PowerIcon className="w-6" />
            <div className="hidden md:block">Sign Out</div>
          </button>
        </form>
      </div>
    </div>
  );
}

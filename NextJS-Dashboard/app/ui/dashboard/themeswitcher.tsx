'use client'

import { 
    SunIcon, 
    MoonIcon, 
} from '@heroicons/react/24/outline';
import { useState, useEffect } from 'react'
import { useTheme } from 'next-themes'

export default function ThemeSwitcher() {
  const [mounted, setMounted] = useState(false);
  const { setTheme, theme } = useTheme();

  useEffect(() => setMounted(true), []);

  if (!mounted) return null;

  if (theme === 'dark') {
    return (
      <div>
        <button className="flex h-[48px] w-full grow items-center justify-center gap-2 rounded-md bg-purple-500 p-3 text-sm font-medium hover:bg-sky-100 hover:text-slate-900 md:flex-none md:justify-start md:p-2 md:px-3"
        onClick={() => setTheme('light')} >
          <SunIcon className="w-6" />
          <div className="hidden md:block">Light Mode</div>
        </button>
      </div>
    )
  } if (theme === 'light') {
    return (
      <div>
        <button className="flex h-[48px] w-full grow items-center justify-center gap-2 rounded-md bg-purple-500 p-3 text-sm font-medium hover:bg-sky-100 hover:text-slate-900 md:flex-none md:justify-start md:p-2 md:px-3"
        onClick={() => setTheme('dark')} >
          <MoonIcon className="w-6" />
          <div className="hidden md:block">Dark Mode</div>
        </button>
      </div>
    )
  } else {
    // failsafe assume light mode, to transition to dark mode
    return (
      <div>
        <button className="flex h-[48px] w-full grow items-center justify-center gap-2 rounded-md bg-purple-500 p-3 text-sm font-medium hover:bg-sky-100 hover:text-slate-900 md:flex-none md:justify-start md:p-2 md:px-3"
        onClick={() => setTheme('dark')} >
          <MoonIcon className="w-6" />
          <div className="hidden md:block">Dark Mode</div>
        </button>
      </div>
    )
  };
}
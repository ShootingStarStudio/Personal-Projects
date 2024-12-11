'use client';

import {
  CheckIcon,
  ClockIcon,
  CurrencyDollarIcon,
  UserCircleIcon,
} from '@heroicons/react/24/outline';
import { createGame } from '@/app/lib/game-actions';
import { useFormState } from 'react-dom';
import Link from 'next/link';
import { Button } from '../button';

export default function Form() {
  const initialState = { message: null, errors: {} };
  const [state, dispatch] = useFormState(createGame, initialState);
  return (
    <form action={dispatch}>
      <div className="rounded-md bg-gray-50 p-4 md:p-6">

        {/* Game Name */}
        <div className="mb-4">
          <label htmlFor="name" className="mb-2 block text-sm font-medium">
            Game Name
          </label>
          <div className="relative mt-2 rounded-md">
            <div className="relative">
              <input
                id="name"
                name="name"
                type="text"
                placeholder="Input Game Name"
                className="peer block w-full rounded-md border border-gray-200 py-2 pl-10 text-sm outline-2 placeholder:text-gray-500"
                required
              />
              <CurrencyDollarIcon className="pointer-events-none absolute left-3 top-1/2 h-[18px] w-[18px] -translate-y-1/2 text-gray-500 peer-focus:text-gray-900" />
            </div>
          </div>
        </div>

        {/* Game Type */}
            <div className="mb-4">
          <label htmlFor="type" className="mb-2 block text-sm font-medium">
            Game Category
          </label>
          <div className="relative mt-2 rounded-md">
            <div className="relative">
              <select
                id="type"
                name="type"
                defaultValue=""
                className="peer block w-full rounded-md border border-gray-200 py-2 pl-10 text-sm outline-2 placeholder:text-gray-500"
                required>
                  <option value="" disabled>Please select a type</option>
                  <option value="fitness">Fitness</option>
                  <option value="cognitive">Cognitive</option>
                  <option value="social">Social</option>
              </select>
              <CurrencyDollarIcon className="pointer-events-none absolute left-3 top-1/2 h-[18px] w-[18px] -translate-y-1/2 text-gray-500 peer-focus:text-gray-900" />
            </div>
          </div>
        </div>

        <fieldset>
          <legend className="mb-2 block text-sm font-medium">
            Sort By
          </legend>
          <div className="rounded-md border border-gray-200 bg-white px-[14px] py-3">
            <div className="flex gap-4">
              <div className="flex items-center">
                <input
                  id="ascending"
                  name="sortby"
                  type="radio"
                  value="ascending"
                  className="h-4 w-4 cursor-pointer border-gray-300 bg-gray-100 text-gray-600 focus:ring-2"
                />
                <label
                  htmlFor="sortby"
                  className="ml-2 flex cursor-pointer items-center gap-1.5 rounded-full bg-gray-100 px-3 py-1.5 text-xs font-medium text-gray-600"
                >
                  Ascending <ClockIcon className="h-4 w-4" />
                </label>
              </div>
              <div className="flex items-center">
                <input
                  id="descending"
                  name="sortby"
                  type="radio"
                  value="descending"
                  className="h-4 w-4 cursor-pointer border-gray-300 bg-gray-100 text-gray-600 focus:ring-2"
                />
                <label
                  htmlFor="sortby"
                  className="ml-2 flex cursor-pointer items-center gap-1.5 rounded-full bg-green-500 px-3 py-1.5 text-xs font-medium text-white"
                >
                  Descending <CheckIcon className="h-4 w-4" />
                </label>
              </div>
            </div>
          </div>
        </fieldset>

        {/* Score Type */}
          <div className="mb-4">
          <label htmlFor="scoretype" className="mb-2 block text-sm font-medium">
            Score Type
          </label>
          <div className="relative mt-2 rounded-md">
            <div className="relative">
              <select
                id="scoretype"
                name="scoretype"
                defaultValue=""
                className="peer block w-full rounded-md border border-gray-200 py-2 pl-10 text-sm outline-2 placeholder:text-gray-500"
                required>
                  <option value="" disabled>Select a Score type</option>
                  <option value="time">Time</option>
                  <option value="float">Float</option>
                  <option value="integer">Integer</option>
              </select>
              <CurrencyDollarIcon className="pointer-events-none absolute left-3 top-1/2 h-[18px] w-[18px] -translate-y-1/2 text-gray-500 peer-focus:text-gray-900" />
            </div>
          </div>
        </div>

        {/* Game Formula */}
        <div className="mb-4">
          <label htmlFor="formula" className="mb-2 block text-sm font-medium">
            Game Formula
          </label>
          <div className="relative mt-2 rounded-md">
            <div className="relative">
              <input
                id="formula"
                name="formula"
                type="text"
                placeholder="Input Game Fomula"
                className="peer block w-full rounded-md border border-gray-200 py-2 pl-10 text-sm outline-2 placeholder:text-gray-500"
                required
              />
              <CurrencyDollarIcon className="pointer-events-none absolute left-3 top-1/2 h-[18px] w-[18px] -translate-y-1/2 text-gray-500 peer-focus:text-gray-900" />
            </div>
          </div>
        </div>
      </div>
      <div className="mt-6 flex justify-end gap-4">
        <Link
          href="/dashboard/games"
          className="flex h-10 items-center rounded-lg bg-gray-100 px-4 text-sm font-medium text-gray-600 transition-colors hover:bg-gray-200"
        >
          Cancel
        </Link>
        <Button type="submit">Confirm</Button>
      </div>
    </form>
  );
}

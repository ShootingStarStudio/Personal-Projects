'use client';

import { Participant } from '../../lib/definitions';
import {
  CheckIcon,
  ClockIcon,
  CurrencyDollarIcon,
  UserCircleIcon,
} from '@heroicons/react/24/outline';
import { createParticipant } from '../../lib/participant-actions';
import { useFormState } from 'react-dom';
import { Button } from '../button';
import Link from 'next/link';
import React from 'react';

export default function Form({ participants }: { participants: Participant[] }) {
  const initialState = { message: null, errors: {} };
  const [state, dispatch] = useFormState(createParticipant, initialState);
  
  return (
    <form action={dispatch}>
      <div className="rounded-md bg-gray-50 p-4 md:p-6">
        {/* Participant Name */}
        <div className="mb-4">
          <label className="mb-2 block text-sm font-medium">
            Participant Name
          </label>
          <div className="relative">
            <input id='name'
                   name='name'
                   type="text" 
                   placeholder='Enter Participant Name'
                   className="peer block w-full rounded-md border border-gray-200 py-2 pl-10 text-sm outline-2 placeholder:text-gray-500"
                   required
           ></input>
            <UserCircleIcon className="pointer-events-none absolute left-3 top-1/2 h-[18px] w-[18px] -translate-y-1/2 text-gray-500" />
          </div>
        </div>

        {/* Center ID */}
        <div className="mb-4">
          <label className="mb-2 block text-sm font-medium">
            Input Center ID
          </label>
          <div className="relative mt-2 rounded-md">
            <div className="relative">
              <input
                id="centerID"
                name="centerID"
                type="text"
                placeholder="Enter Center ID"
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
          href="/dashboard/participants"
          className="flex h-10 items-center rounded-lg bg-gray-100 px-4 text-sm font-medium text-gray-600 transition-colors hover:bg-gray-200"
        >
          Cancel
        </Link>
        <Button type="submit">Confirm</Button>
      </div>
    </form>
  );
}

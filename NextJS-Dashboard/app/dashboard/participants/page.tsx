import Pagination from '../../ui/pagination';
import Search from '../..//ui/search';
import Table from '../../ui/participants/table';
import { CreateParticipant } from '../../ui/participants/buttonlogic';
import { lusitana } from '../../ui/fonts';
import { SessionsTableSkeleton } from '../../ui/skeletons';
import { Suspense } from 'react';
import { fetchParticipantsPages } from '../../lib/data';
import { Metadata } from 'next';
import React from 'react';

export const metadata: Metadata = {
  title: 'Participants',
};

export default async function Page({
  searchParams,
}: {
  searchParams?: {
    query?: string;
    page?: string;
  };
}) {
  const query = searchParams?.query || '';
  const currentPage = Number(searchParams?.page) || 1;

  const totalPages = await fetchParticipantsPages(query);

  return (
    <div className="w-full">
      <div className="flex w-full items-center justify-between">
        <h1 className={`${lusitana.className} text-2xl`}>Participants</h1>
      </div>
      <div className="mt-4 flex items-center justify-between gap-2 md:mt-8">
        <Search placeholder="Search participants..." />
        <CreateParticipant />
      </div>
       <Suspense key={query + currentPage} fallback={<SessionsTableSkeleton />}>
        <Table query={query} currentPage={currentPage} />
      </Suspense>
      <div className="mt-5 flex w-full justify-center">
        <Pagination totalPages={totalPages} />
      </div>
    </div>
  );
}
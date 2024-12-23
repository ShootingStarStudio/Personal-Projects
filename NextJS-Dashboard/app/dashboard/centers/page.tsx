import Pagination from '@/app/ui/pagination';
import Search from '@/app/ui/search';
import Table from '@/app/ui/centers/table';
import { CreateCenter } from '@/app/ui/centers/buttonlogic';
import { lusitana } from '@/app/ui/fonts';
import { SessionsTableSkeleton } from '@/app/ui/skeletons';
import { Suspense } from 'react';
import { fetchCenterPages } from '@/app/lib/data';
import { Metadata } from 'next';
export const metadata: Metadata = {
  title: 'Centers',
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

  const totalPages = await fetchCenterPages(query);

  return (
    <div className="w-full">
      <div className="flex w-full items-center justify-between">
        <h1 className={`${lusitana.className} text-2xl`}>Centers</h1>
      </div>
      <div className="mt-4 flex items-center justify-between gap-2 md:mt-8">
        <Search placeholder="Search Centers..." />
        <CreateCenter />
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
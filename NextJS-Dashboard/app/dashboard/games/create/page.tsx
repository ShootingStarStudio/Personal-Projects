import Form from '@/app/ui/games/create-form';
import Breadcrumbs from '@/app/ui/breadcrumbs';
import { fetchParticipants } from '@/app/lib/data';
import { Metadata } from 'next';
 
export const metadata: Metadata = {
  title: 'Games Create',
};

export default async function Page() {
  const participants = await fetchParticipants();
 
  return (
    <>
      <Breadcrumbs
        breadcrumbs={[
          { label: 'Games', href: '/dashboard/games' },
          {
            label: 'Create Games',
            href: '/dashboard/games/create',
            active: true,
          },
        ]}
      />
      <Form participants={participants} />
    </>
  );
}
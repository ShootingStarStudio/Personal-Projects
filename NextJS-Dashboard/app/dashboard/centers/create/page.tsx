import Form from '@/app/ui/centers/create-form';
import Breadcrumbs from '@/app/ui/breadcrumbs';
import { fetchParticipants } from '@/app/lib/data';
import { Metadata } from 'next';
 
export const metadata: Metadata = {
  title: 'Centers Create',
};

export default async function Page() {
  const participants = await fetchParticipants();
 
  return (
    <>
      <Breadcrumbs
        breadcrumbs={[
          { label: 'Centers', href: '/dashboard/centers' },
          {
            label: 'Create Center',
            href: '/dashboard/centers/create',
            active: true,
          },
        ]}
      />
      <Form />
      {/* pending inputs, undone function */}
    </>
  );
}
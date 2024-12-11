import Form from '../../../ui/participants/create-form';
import Breadcrumbs from '../../../ui/breadcrumbs';
import { fetchParticipants } from '../../../lib/data';
import { Metadata } from 'next';
 
export const metadata: Metadata = {
  title: 'Participants Create',
};

export default async function Page() {
  const participants = await fetchParticipants();
 
  return (
    <main>
      <Breadcrumbs
        breadcrumbs={[
          { label: 'Participants', href: '/dashboard/participants' },
          {
            label: 'Create Participant',
            href: '/dashboard/participants/create',
            active: true,
          },
        ]}
      />
      <Form participants={participants} />
    </main>
  );
}
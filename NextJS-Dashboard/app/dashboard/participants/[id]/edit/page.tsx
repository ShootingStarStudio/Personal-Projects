import Form from '@/app/ui/participants/edit-form';
import Breadcrumbs from '@/app/ui/breadcrumbs';
import { 
    fetchParticipantById
} from '@/app/lib/data';
import { notFound } from 'next/navigation';
import { Metadata } from 'next';
 
export const metadata: Metadata = {
  title: 'Participants Edit',
};

export default async function Page(
    { params }: { params: { id: string } }
) {
    const id = params.id;
    const [participant] = await Promise.all([
        fetchParticipantById(id),
      ]);
      
      if (!participant) {
        notFound();
      }

  return (
    <>
      <Breadcrumbs
        breadcrumbs={[
          { label: 'Participants', href: '/dashboard/participants' },
          {
            label: 'Edit Participants',
            href: `/dashboard/participants/${id}/edit`,
            active: true,
          },
        ]}
      />
      <Form participant={participant} />
    </>
  );
}
import Form from '@/app/ui/sessions/edit-form';
import Breadcrumbs from '@/app/ui/breadcrumbs';
import { 
    fetchSessionById,
} from '@/app/lib/data';
import { notFound } from 'next/navigation';
import { Metadata } from 'next';
 
export const metadata: Metadata = {
  title: 'Sessions Edit',
};

export default async function Page(
    { params }: { params: { id: string } }
) {
    const id = params.id;
    const [session] = await Promise.all([
      fetchSessionById(id),
    ]);
      
    if (!session) {
      notFound();
    }

  return (
    <>
      <Breadcrumbs
        breadcrumbs={[
          { label: 'Sessions', href: '/dashboard/sessions' },
          {
            label: 'Edit Session',
            href: `/dashboard/sessions/${id}/edit`,
            active: true,
          },
        ]}
      />
      <Form session={session} />
    </>
  );
}
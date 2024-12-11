import Form from '@/app/ui/centers/edit-form';
import Breadcrumbs from '@/app/ui/breadcrumbs';
import { 
    fetchCenterById,
} from '@/app/lib/data';
import { notFound } from 'next/navigation';
import { Metadata } from 'next';
 
export const metadata: Metadata = {
  title: 'Centers Edit',
};

export default async function Page(
    { params }: { params: { id: string } }
) {
    const id = params.id;
    const [center] = await Promise.all([
        fetchCenterById(id),
      ]);
      
    if (!center) {
      notFound();
    }

  return (
    <>
      <Breadcrumbs
        breadcrumbs={[
          { label: 'Centers', href: '/dashboard/centers' },
          {
            label: 'Edit Center',
            href: `/dashboard/centers/${id}/edit`,
            active: true,
          },
        ]}
      />
      <Form center={center} />
    </>
  );
}
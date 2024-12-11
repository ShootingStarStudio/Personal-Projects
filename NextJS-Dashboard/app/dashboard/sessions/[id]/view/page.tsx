import Breadcrumbs from '@/app/ui/breadcrumbs';
import { 
    fetchSessionById, 
    fetchParticipants 
} from '@/app/lib/data';
import { notFound } from 'next/navigation';
import { Metadata } from 'next';

//scoreboard with calculation of the scores with game formula
//can edit the participant data
//closes the session as well
//can be filtered by score of each segment

export const metadata: Metadata = {
  title: 'Sessions View',
};

export default async function Page(
    { params }: { params: { id: string } }
) {
    const id = params.id;
    const [session, participants] = await Promise.all([
        fetchSessionById(id),
        fetchParticipants(),
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
            label: 'View Session',
            href: `/dashboard/sessions/${id}/view`,
            active: true,
          },
        ]}
      />
      {/* Removed the form here, expected screen to be empty with breadcrumbs */}
      {/* render the scoreboard here, allow for filtering by score or by participant */}
    </>
  );
}
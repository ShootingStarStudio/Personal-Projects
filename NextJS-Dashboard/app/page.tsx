import AriseLogo from '@/app/ui/arise-logo';
import LoginForm from '@/app/ui/login-form';
import { Metadata } from 'next';
 
export const metadata: Metadata = {
  title: 'Index - Login',
};

export default function Page() {
  return (
    <main className="flex items-center justify-center md:h-screen">
      <div className="relative mx-auto flex w-full max-w-[400px] flex-col space-y-2.5 p-4 md:-mt-32">
        <div className="flex h-20 w-full items-end rounded-lg bg-purple-700 p-3 md:h-36">
          <div className="w-32 text-white md:w-36">
            <AriseLogo />
          </div>
        </div>
        <LoginForm />
      </div>
    </main>
  );
}
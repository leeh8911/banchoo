import { useNavigate, useLocation } from 'react-router-dom';

const NavigationBar = () => {
    const navigate = useNavigate();
    const location = useLocation();

    const isActive = (path: string) => location.pathname.startsWith(path);

    return (
        <div className="fixed bottom-0 left-0 right-0 bg-white dark:bg-black border-t border-gray-200 dark:border-gray-700 flex justify-around py-3 text-sm font-medium z-50">
            <button onClick={() => navigate(-1)} className="text-gray-600 dark:text-gray-300">
                ⬅️ 뒤로
            </button>
            <button onClick={() => navigate('/')} className={isActive('/') ? 'text-blue-500' : ''}>
                🏠 홈
            </button>
            <button onClick={() => navigate('/memos')} className={isActive('/memos') ? 'text-blue-500' : ''}>
                📝 메모
            </button>
            <button onClick={() => navigate('/tasks')} className={isActive('/tasks') ? 'text-blue-500' : ''}>
                ✅ 할 일
            </button>
            <button onClick={() => navigate('/events')} className={isActive('/events') ? 'text-blue-500' : ''}>
                📅 일정
            </button>
        </div>
    );
};

export default NavigationBar;

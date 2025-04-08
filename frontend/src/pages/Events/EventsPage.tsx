import { useEffect, useState } from 'react';
import { useNavigate } from 'react-router-dom';
import { Event } from '@/types/Event';

const EventsPage = () => {
    const [events, setEvents] = useState<Event[]>([]);
    const navigate = useNavigate();

    useEffect(() => {
        const saved = JSON.parse(localStorage.getItem('events') || '[]') as Event[];
        const sorted = saved.sort(
            (a, b) => new Date(b.updatedAt).getTime() - new Date(a.updatedAt).getTime()
        );
        setEvents(sorted);
    }, []);

    return (
        <div className="min-h-screen p-4 max-w-screen-md mx-auto text-gray-900 dark:text-white">
            <div className="flex justify-between items-center mb-4">
                <h1 className="text-2xl font-bold">📅 일정 목록</h1>
                <button
                    onClick={() => navigate('/events/new')}
                    className="text-blue-600 font-semibold"
                >
                    생성
                </button>
            </div>

            {events.length === 0 ? (
                <p className="text-gray-400">등록된 일정이 없습니다.</p>
            ) : (
                <ul className="space-y-3">
                    {events.map((event) => (
                        <li
                            key={event.id}
                            className="p-4 rounded-lg border hover:shadow transition flex items-start justify-between gap-3 bg-white dark:bg-gray-800"
                        >
                            <div
                                onClick={() => navigate(`/events/${event.id}`)}
                                className="flex-1 cursor-pointer"
                            >
                                <p className="text-sm whitespace-pre-wrap">{event.content}</p>
                                <p className="mt-1 text-xs text-gray-400">
                                    {event.startDate
                                        ? `시작: ${new Date(event.startDate).toLocaleString()}`
                                        : '시작일 미정'}
                                    {event.endDate
                                        ? ` ~ 종료: ${new Date(event.endDate).toLocaleString()}`
                                        : ''}
                                </p>
                            </div>
                        </li>
                    ))}
                </ul>
            )}
        </div>
    );
};

export default EventsPage;
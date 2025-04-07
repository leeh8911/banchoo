import { useEffect, useState } from 'react';
import { useNavigate } from 'react-router-dom';

interface Task {
    id: string;
    content: string;
    createdAt: string;
    updatedAt: string;
    status: 'todo' | 'done';
}

const extractTags = (text: string): string[] => {
    const matches = text.match(/#\w+/g);
    return matches ? matches.map((t) => t.slice(1)) : [];
};

const TasksPage = () => {
    const [tasks, setTasks] = useState<Task[]>([]);
    const [showDone, setShowDone] = useState(false);
    const navigate = useNavigate();

    useEffect(() => {
        const saved = JSON.parse(localStorage.getItem('tasks') || '[]') as Task[];
        const sorted = saved.sort(
            (a, b) => new Date(b.updatedAt).getTime() - new Date(a.updatedAt).getTime()
        );
        setTasks(sorted);
    }, []);

    const toggleStatus = (id: string) => {
        const updatedTasks = tasks.map((task) =>
            task.id === id
                ? {
                    ...task,
                    status: task.status === 'todo' ? 'done' : 'todo',
                    updatedAt: new Date().toISOString(),
                }
                : task
        );
        setTasks(updatedTasks);
        localStorage.setItem('tasks', JSON.stringify(updatedTasks));
    };

    return (
        <div className="min-h-screen p-4 max-w-screen-md mx-auto text-gray-900 dark:text-white">
            <div className="flex justify-between items-center mb-4">
                <h1 className="text-2xl font-bold">✅ 할 일 목록</h1>
                <button
                    onClick={() => navigate('/tasks/new')}
                    className="text-green-600 font-semibold"
                >
                    생성
                </button>
            </div>

            <div className="mb-4 flex items-center justify-between">
                <label className="flex items-center gap-2">
                    <input
                        type="checkbox"
                        checked={showDone}
                        onChange={() => setShowDone((prev) => !prev)}
                    />
                    완료된 항목 보기
                </label>
            </div>

            <ul className="space-y-3">
                {tasks
                    .filter((task) => showDone || task.status !== 'done')
                    .map((task) => (
                        <li
                            key={task.id}
                            className="p-4 rounded-lg border hover:shadow transition flex items-center justify-between gap-3 bg-white dark:bg-gray-800"
                        >
                            <div
                                onClick={() => navigate(`/tasks/${task.id}`)}
                                className="flex-1 cursor-pointer"
                            >
                                <p className={`text-sm whitespace-pre-wrap ${task.status === 'done' ? 'line-through text-gray-400' : ''}`}>
                                    {task.content}
                                </p>
                                <p className="mt-1 text-xs text-gray-400">
                                    수정일: {new Date(task.updatedAt).toLocaleString()}
                                </p>
                            </div>
                            <input
                                type="checkbox"
                                checked={task.status === 'done'}
                                onChange={() => toggleStatus(task.id)}
                            />
                        </li>
                    ))}
            </ul>
        </div>
    );
};

export default TasksPage;
import { useParams, useNavigate } from 'react-router-dom';
import { useEffect, useState } from 'react';
import { Memo } from '@/types/note';


const MemoDetailPage = () => {
    const { id } = useParams<{ id: string }>();
    const navigate = useNavigate();
    const [memo, setMemo] = useState<Memo | null>(null);

    useEffect(() => {
        const saved = JSON.parse(localStorage.getItem('memos') || '[]') as Memo[];
        const found = saved.find((m) => m.id === id);
        if (found) setMemo(found);
    }, [id]);

    const handleUpdate = (newContent: string) => {
        if (!memo) return;
        const updated: Memo = {
            ...memo,
            content: newContent,
            updatedAt: new Date().toISOString(),
        };

        const saved = JSON.parse(localStorage.getItem('memos') || '[]') as Memo[];
        const newList = saved.map((m) => (m.id === memo.id ? updated : m));
        localStorage.setItem('memos', JSON.stringify(newList));
        setMemo(updated);
    };

    if (!memo) {
        return (
            <div className="min-h-screen p-6 max-w-screen-md mx-auto text-gray-500 dark:text-gray-400">
                존재하지 않는 메모입니다.
                <button
                    onClick={() => navigate('/memos')}
                    className="mt-4 text-blue-500 underline"
                >
                    목록으로 돌아가기
                </button>
            </div>
        );
    }

    return (
        <div className="min-h-screen p-6 max-w-screen-md mx-auto text-gray-900 dark:text-white">
            <h1 className="text-2xl font-bold mb-4">📝 메모 편집</h1>
            <textarea
                value={memo.content}
                onChange={(e) => handleUpdate(e.target.value)}
                className="w-full h-[70vh] resize-none p-4 text-base leading-relaxed border border-gray-300 dark:border-gray-600 bg-white dark:bg-gray-900 rounded-md shadow focus:outline-none focus:ring-2 focus:ring-blue-500"
            />
            <p className="mt-4 text-xs text-gray-400">
                수정일: {new Date(memo.updatedAt).toLocaleString()}
            </p>
        </div>
    );
};

export default MemoDetailPage;

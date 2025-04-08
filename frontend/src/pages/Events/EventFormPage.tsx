import { useEffect, useRef, useState } from 'react';
import { useNavigate } from 'react-router-dom';
import { Event } from '@/types/Event';

interface Transaction {
    type: 'insert' | 'space' | 'newline';
    value?: string;
    index: number;
}

const EventFormPage = () => {
    const [transactions, setTransactions] = useState<Transaction[]>([]);
    const [content, setContent] = useState('');
    const [eventId] = useState(() => crypto.randomUUID());
    const [startDate, setStartDate] = useState<string>('');
    const [endDate, setEndDate] = useState<string>('');
    const textareaRef = useRef<HTMLTextAreaElement>(null);
    const navigate = useNavigate();

    const handleKeyDown = (e: React.KeyboardEvent<HTMLTextAreaElement>) => {
        const cursor = textareaRef.current?.selectionStart ?? 0;

        if (e.key === ' ' || e.key === 'Enter') {
            const type = e.key === ' ' ? 'space' : 'newline';
            if (content.length > 0) {
                setTransactions((prev) => [...prev, { type, index: cursor }]);
            }
        }
    };

    const handleChange = (e: React.ChangeEvent<HTMLTextAreaElement>) => {
        const cursor = textareaRef.current?.selectionStart ?? 0;
        const newContent = e.target.value;

        if (newContent.length > content.length) {
            const added = newContent.slice(content.length);
            setTransactions((prev) => [
                ...prev,
                { type: 'insert', value: added, index: cursor - added.length },
            ]);
        }

        setContent(newContent);
    };

    const commit = () => {
        let committedTransactions = [...transactions];
        const reconstructedText = reconstruct(committedTransactions);

        if (reconstructedText !== content) {
            committedTransactions = [];
            for (let i = 0; i < content.length; i++) {
                committedTransactions.push({
                    type: 'insert',
                    value: content[i],
                    index: i,
                });
            }
        }

        const text = reconstruct(committedTransactions);
        if (!text.trim() || text.length < 2) return;

        const newEvent = {
            id: eventId,
            content: text,
            createdAt: new Date().toISOString(),
            updatedAt: new Date().toISOString(),
            startDate: startDate || undefined,
            endDate: endDate || undefined,
        };

        const existing = JSON.parse(localStorage.getItem('events') || '[]');
        if (existing.find((e: any) => e.id === eventId)) return;
        localStorage.setItem('events', JSON.stringify([...existing, newEvent]));
    };

    useEffect(() => {
        return () => {
            commit();
        };
    }, [transactions, content]);

    const reconstruct = (txs: Transaction[]) => {
        let result = '';
        for (const tx of txs) {
            if (tx.type === 'insert' && tx.value) {
                result = result.slice(0, tx.index) + tx.value + result.slice(tx.index);
            } else if (tx.type === 'space') {
                result = result.slice(0, tx.index) + ' ' + result.slice(tx.index);
            } else if (tx.type === 'newline') {
                result = result.slice(0, tx.index) + '\n' + result.slice(tx.index);
            }
        }
        return result;
    };

    return (
        <div className="min-h-screen bg-white dark:bg-black text-gray-900 dark:text-white flex flex-col">
            <div className="flex-1 px-4 pt-6 max-w-screen-md mx-auto w-full pb-24">
                <h1 className="text-2xl font-bold mb-4">📅 새 일정</h1>

                <label className="block mb-2 font-semibold">시작일</label>
                <input
                    type="datetime-local"
                    value={startDate}
                    onChange={(e) => setStartDate(e.target.value)}
                    className="mb-4 w-full p-2 border rounded dark:bg-gray-900"
                />

                <label className="block mb-2 font-semibold">종료일</label>
                <input
                    type="datetime-local"
                    value={endDate}
                    onChange={(e) => setEndDate(e.target.value)}
                    className="mb-4 w-full p-2 border rounded dark:bg-gray-900"
                />

                <textarea
                    ref={textareaRef}
                    value={content}
                    onChange={handleChange}
                    onKeyDown={handleKeyDown}
                    placeholder="일정 내용을 입력하세요. 예: 회의, 약속, 프로젝트 시작 등"
                    className="w-full h-[60vh] resize-none p-4 text-base leading-relaxed border border-gray-300 dark:border-gray-600 bg-white dark:bg-gray-900 rounded-lg shadow focus:outline-none focus:ring-2 focus:ring-blue-500"
                />
            </div>
        </div>
    );
};

export default EventFormPage;
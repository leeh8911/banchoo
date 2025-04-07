import { useEffect, useRef, useState } from 'react';
import { useNavigate } from 'react-router-dom';

interface Transaction {
    type: 'insert' | 'space' | 'newline';
    value?: string;
    index: number;
}

const TaskFormPage = () => {
    const [transactions, setTransactions] = useState<Transaction[]>([]);
    const [content, setContent] = useState('');
    const [taskId] = useState(() => crypto.randomUUID());
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

        const newTask = {
            id: taskId,
            content: text,
            createdAt: new Date().toISOString(),
            updatedAt: new Date().toISOString(),
            status: 'todo',
        };

        const existing = JSON.parse(localStorage.getItem('tasks') || '[]');
        if (existing.find((t: any) => t.id === taskId)) return;
        localStorage.setItem('tasks', JSON.stringify([...existing, newTask]));
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
                <h1 className="text-2xl font-bold mb-4">✅ 새 할 일</h1>
                <textarea
                    ref={textareaRef}
                    value={content}
                    onChange={handleChange}
                    onKeyDown={handleKeyDown}
                    placeholder="할 일을 입력하세요. 예: 프로젝트 문서 작성하기"
                    className="w-full h-[70vh] resize-none p-4 text-base leading-relaxed border border-gray-300 dark:border-gray-600 bg-white dark:bg-gray-900 rounded-lg shadow focus:outline-none focus:ring-2 focus:ring-blue-500"
                />
            </div>
        </div>
    );
};

export default TaskFormPage;

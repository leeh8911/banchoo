import { useEffect, useRef, useState } from 'react';
import { useNavigate } from 'react-router-dom';

// 트랜잭션 타입 정의
interface Transaction {
    type: 'insert' | 'space' | 'newline';
    value?: string;
    index: number;
}

const MemoFormPage = () => {
    const [transactions, setTransactions] = useState<Transaction[]>([]);
    const [content, setContent] = useState('');
    const [memoId] = useState(() => crypto.randomUUID());
    const textareaRef = useRef<HTMLTextAreaElement>(null);
    const navigate = useNavigate();

    // 커서 위치 기반으로 트랜잭션 기록
    const handleKeyDown = (e: React.KeyboardEvent<HTMLTextAreaElement>) => {
        const cursor = textareaRef.current?.selectionStart ?? 0;

        if (e.key === ' ' || e.key === 'Enter') {
            const type = e.key === ' ' ? 'space' : 'newline';
            if (content.length > 0) {
                setTransactions((prev) => [
                    ...prev,
                    { type, index: cursor },
                ]);
            }
        }
    };

    const handleChange = (e: React.ChangeEvent<HTMLTextAreaElement>) => {
        const cursor = textareaRef.current?.selectionStart ?? 0;
        const newContent = e.target.value;

        // 변경된 글자만 추출
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

        // 누락된 입력 계산
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

        const newMemo = {
            id: memoId,
            content: text,
            createdAt: new Date().toISOString(),
            updatedAt: new Date().toISOString(),
        };

        const existing = JSON.parse(localStorage.getItem('memos') || '[]');
        if (existing.find((m: any) => m.id === memoId)) return;
        localStorage.setItem('memos', JSON.stringify([...existing, newMemo]));
    };

    // 언마운트 시점에 commit
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
                <h1 className="text-2xl font-bold mb-4">📝 새 메모 (트랜잭션 기반)</h1>
                <textarea
                    ref={textareaRef}
                    value={content}
                    onChange={handleChange}
                    onKeyDown={handleKeyDown}
                    placeholder="#태그를 포함해 메모를 작성하세요."
                    className="w-full h-[70vh] resize-none p-4 text-base leading-relaxed border border-gray-300 dark:border-gray-600 bg-white dark:bg-gray-900 rounded-lg shadow focus:outline-none focus:ring-2 focus:ring-blue-500"
                />
            </div>
        </div>
    );
};

export default MemoFormPage;
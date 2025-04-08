import { useState, useRef, useEffect } from 'react';

export interface Transaction {
    type: 'insert' | 'space' | 'newline';
    value?: string;
    index: number;
}

export function useTransactionText() {
    const [content, setContent] = useState('');
    const [transactions, setTransactions] = useState<Transaction[]>([]);
    const textareaRef = useRef<HTMLTextAreaElement>(null);

    const contentRef = useRef('');
    const transactionsRef = useRef<Transaction[]>([]);

    // 항상 최신 상태를 ref에 복사
    useEffect(() => {
        contentRef.current = content;
    }, [content]);

    useEffect(() => {
        transactionsRef.current = transactions;
    }, [transactions]);

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

        const delta = newContent.length - content.length;
        const added = delta > 0 ? newContent.slice(-delta) : '';

        if (added) {
            setTransactions((prev) => [
                ...prev,
                { type: 'insert', value: added, index: cursor - added.length },
            ]);
        }

        console.log('newContent', newContent);
        setContent(newContent);
    };

    const reconstruct = (txs = transactions) => {
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

    // ref도 외부에서 사용할 수 있게 리턴
    return {
        content,
        setContent,
        transactions,
        textareaRef,
        contentRef,
        transactionsRef,
        handleKeyDown,
        handleChange,
        reconstruct,
    };
}

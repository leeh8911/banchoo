import { useEffect } from 'react';
import { useNavigate } from 'react-router-dom';
import { Memo } from '@/types/note';
import { useTransactionText } from '@/hooks/useTransactionText';
import { createMemo } from '@/lib/api';

const MemoFormPage = () => {
    const navigate = useNavigate();
    const {
        content,
        setContent,
        transactions,
        textareaRef,
        contentRef,
        transactionsRef,
        handleKeyDown,
        handleChange,
        reconstruct,
    } = useTransactionText();

    const commit = async () => {
        const currentContent = contentRef.current;
        let committedTransactions = [...transactionsRef.current];

        const reconstructedText = reconstruct(committedTransactions);

        // 누락된 입력 계산
        if (reconstructedText !== currentContent) {
            committedTransactions = [];
            for (let i = 0; i < currentContent.length; i++) {
                committedTransactions.push({
                    type: 'insert',
                    value: currentContent[i],
                    index: i,
                });
            }
        }

        const text = reconstruct(committedTransactions);

        if (!text.trim() || text.length < 2) return;

        try {
            await createMemo(text); // 백엔드로 메모 전송
            console.log("memo saved to backend");
        } catch (error) {
            console.error("failed to save memo", error);
        }
    };

    // 언마운트 시점에만 저장하도록 수정
    useEffect(() => {
        const handleBeforeUnload = () => commit();
        window.addEventListener('beforeunload', handleBeforeUnload);
        return () => {
            window.removeEventListener('beforeunload', handleBeforeUnload);
            commit();
        };
    }, []);

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
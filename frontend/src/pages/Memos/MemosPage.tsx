import { useEffect, useState } from 'react';
import { useNavigate } from 'react-router-dom';
import { Memo } from '@/types/note';
import { fetchMemos } from '@/lib/api';

const MemosPage = () => {
    const [memos, setMemos] = useState<Memo[]>([]);
    const [selectionMode, setSelectionMode] = useState(false);
    const [selectedIds, setSelectedIds] = useState<Set<string>>(new Set());

    const navigate = useNavigate();

    useEffect(() => {
        loadMemos();
    }, []);

    const loadMemos = async () => {
        try {
            const result = await fetchMemos(); // ✅ 서버에서 불러오기
            const sorted = result.sort(
                (a, b) => new Date(b.updatedAt).getTime() - new Date(a.updatedAt).getTime()
            );
            setMemos(sorted);
        } catch (error) {
            console.error('메모 불러오기 실패', error);
        }
    };

    const toggleSelection = (id: string) => {
        setSelectedIds((prev) => {
            const newSet = new Set(prev);
            newSet.has(id) ? newSet.delete(id) : newSet.add(id);
            return newSet;
        });
    };

    const deleteSelected = () => {
        const remaining = memos.filter((memo) => !selectedIds.has(memo.id));
        localStorage.setItem('memos', JSON.stringify(remaining));
        setSelectionMode(false);
        setSelectedIds(new Set());
        setMemos(remaining);
    };

    const cancelSelection = () => {
        setSelectionMode(false);
        setSelectedIds(new Set());
    };

    return (
        <div className="min-h-screen p-4 max-w-screen-md mx-auto text-gray-900 dark:text-white">
            <div className="flex justify-between items-center mb-6">
                <h1 className="text-2xl font-bold">📚 메모 목록</h1>
                {!selectionMode ? (
                    <div className="flex gap-2">
                        <button
                            onClick={() => setSelectionMode(true)}
                            className="text-blue-500 font-semibold"
                        >
                            선택
                        </button>
                        <button
                            onClick={() => navigate('/memos/new')}
                            className="text-green-600 font-semibold"
                        >
                            생성
                        </button>
                    </div>
                ) : (
                    <div className="flex gap-2">
                        <button onClick={deleteSelected} className="text-red-500 font-semibold">
                            삭제
                        </button>
                        <button onClick={cancelSelection} className="text-gray-500 font-semibold">
                            취소
                        </button>
                    </div>
                )}
            </div>

            {memos.length === 0 ? (
                <p className="text-gray-400">저장된 메모가 없습니다.</p>
            ) : (
                <ul className="space-y-3">
                    {memos.map((memo) => (
                        <li
                            key={memo.id}
                            className={`p-4 rounded-lg border hover:shadow transition flex items-start ${selectionMode ? 'gap-3' : 'cursor-pointer'
                                }`}
                            onClick={() => !selectionMode && navigate(`/memos/${memo.id}`)}
                        >
                            {selectionMode && (
                                <input
                                    type="checkbox"
                                    checked={selectedIds.has(memo.id)}
                                    onChange={() => toggleSelection(memo.id)}
                                    onClick={(e) => e.stopPropagation()} // 클릭 버블링 방지
                                />
                            )}
                            <div className="flex-1 whitespace-pre-wrap line-clamp-3 text-sm">
                                {memo.content}
                                <div className="mt-2 text-xs text-gray-400">
                                    수정일: {new Date(memo.updatedAt).toLocaleString()}
                                </div>
                            </div>
                        </li>
                    ))}
                </ul>
            )}
        </div>
    );
};

export default MemosPage;

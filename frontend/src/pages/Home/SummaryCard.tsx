import { FC } from 'react';
import { PlusIcon } from 'lucide-react';
import { useNavigate } from 'react-router-dom';

type SummaryCardProps = {
    title: string;
    count: number;
    color: string;
    type: 'memos' | 'tasks' | 'events';
};

const SummaryCard: FC<SummaryCardProps> = ({ title, count, color, type }) => {
    const navigate = useNavigate();

    return (
        <div
            className={`flex justify-between items-center p-4 rounded-xl shadow w-full text-gray-900`}
            style={{ backgroundColor: color }}
        >
            <div onClick={() => navigate(`/${type}`)} className="cursor-pointer">
                <h2 className="text-lg sm:text-xl font-semibold">{title}</h2>
                <p className="text-sm">{count} 개</p>
            </div>
            <button
                onClick={() => navigate(`/${type}/new`)}
                className="bg-black/70 text-white px-3 py-1 rounded-full hover:bg-black/90"
            >
                +
            </button>
        </div>

    );
};

export default SummaryCard;

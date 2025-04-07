import SummaryCard from './SummaryCard';

const HomePage = () => {
    // 실제 데이터는 API 연동 예정. 지금은 mock
    const summaries = [
        { title: '메모', count: 5, color: '#e0f2fe', type: 'memos' },
        { title: '할 일', count: 3, color: '#dcfce7', type: 'tasks' },
        { title: '일정', count: 2, color: '#ede9fe', type: 'events' },
    ];

    return (
        <div className="p-4 space-y-4">
            {summaries.map((s) => (
                <SummaryCard key={s.type} {...s} />
            ))}
        </div>
    );
};

export default HomePage;

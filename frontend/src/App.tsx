
import { useEffect, useState } from 'react';
import {
  createMemo,
  createTask,
  createEvent,
  getMemos,
  getTasks,
  getEvents,
  Note
} from './api';

function App() {
  const [memoContent, setMemoContent] = useState('');
  const [taskContent, setTaskContent] = useState('');
  const [eventContent, setEventContent] = useState('');
  const [eventStart, setEventStart] = useState('');
  const [eventEnd, setEventEnd] = useState('');

  const [memos, setMemos] = useState<Note[]>([]);
  const [tasks, setTasks] = useState<Note[]>([]);
  const [events, setEvents] = useState<Note[]>([]);

  const handleCreateMemo = async () => {
    const res = await createMemo(memoContent);
    setMemoContent('');
    fetchData();
  };

  const handleCreateTask = async () => {
    await createTask({ content: taskContent });
    setTaskContent('');
    fetchData();
  };

  const handleCreateEvent = async () => {
    await createEvent({
      content: eventContent,
      start_date: eventStart,
      end_date: eventEnd,
    });
    setEventContent('');
    setEventStart('');
    setEventEnd('');
    fetchData();
  };

  const fetchData = async () => {
    const [memoList, taskList, eventList] = await Promise.all([
      getMemos(),
      getTasks(),
      getEvents(),
    ]);
    setMemos(memoList);
    setTasks(taskList);
    setEvents(eventList);
  };

  useEffect(() => {
    fetchData();
  }, []);

  return (
    <div style={{ padding: 40, fontFamily: 'sans-serif' }}>
      <h1>Banchoo Note Manager</h1>

      <section>
        <h2>📝 메모 작성</h2>
        <textarea
          rows={3}
          style={{ width: '100%' }}
          value={memoContent}
          onChange={(e) => setMemoContent(e.target.value)}
        />
        <br />
        <button onClick={handleCreateMemo}>저장</button>
      </section>

      <section>
        <h2>✅ 할 일 추가</h2>
        <textarea
          rows={3}
          style={{ width: '100%' }}
          value={taskContent}
          onChange={(e) => setTaskContent(e.target.value)}
        />
        <br />
        <button onClick={handleCreateTask}>추가</button>
      </section>

      <section>
        <h2>📅 이벤트 등록</h2>
        <input
          type="text"
          placeholder="내용"
          style={{ width: '100%' }}
          value={eventContent}
          onChange={(e) => setEventContent(e.target.value)}
        />
        <input
          type="datetime-local"
          style={{ width: '100%' }}
          value={eventStart}
          onChange={(e) => setEventStart(e.target.value)}
        />
        <input
          type="datetime-local"
          style={{ width: '100%' }}
          value={eventEnd}
          onChange={(e) => setEventEnd(e.target.value)}
        />
        <br />
        <button onClick={handleCreateEvent}>등록</button>
      </section>

      <hr />

      <section>
        <h2>📋 메모 목록</h2>
        <ul>
          {memos.map((m) => (
            <li key={m.id}>{m.content}</li>
          ))}
        </ul>
      </section>

      <section>
        <h2>📌 할 일 목록</h2>
        <ul>
          {tasks.map((t) => (
            <li key={t.id}>{t.content}</li>
          ))}
        </ul>
      </section>

      <section>
        <h2>📆 이벤트 목록</h2>
        <ul>
          {events.map((e) => (
            <li key={e.id}>
              {e.content} ({e.start_date} ~ {e.end_date})
            </li>
          ))}
        </ul>
      </section>
    </div>
  );
}

export default App;
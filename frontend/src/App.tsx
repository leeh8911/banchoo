import { BrowserRouter, Routes, Route } from 'react-router-dom';
import HomePage from './pages/Home/HomePage';
import MemoFormPage from './pages/Memos/MemoFormPage';
import MemosPage from './pages/Memos/MemosPage';
import MemoDetailPage from './pages/Memos/MemoDetailPage';
import TaskFormPage from './pages/Tasks/TaskFormPage';
import TasksPage from './pages/Tasks/TasksPage';
import EventFormPage from './pages/Events/EventFormPage';
import EventsPage from './pages/Events/EventsPage';
import NavigationBar from './components/navigation/NavigationBar';
// import MemosPage, TasksPage, EventsPage 등도 여기에 추가 가능

const App = () => {
  return (
    <BrowserRouter>
      <div className="pb-20"> {/* 하단 바 가릴 방지용 padding */}
        <Routes>
          <Route path="/" element={<HomePage />} />
          <Route path="/memos/new" element={<MemoFormPage />} />
          <Route path="/memos" element={<MemosPage />} />
          <Route path="/memos/:id" element={<MemoDetailPage />} />
          <Route path="/tasks/new" element={<TaskFormPage />} />
          <Route path="/tasks" element={<TasksPage />} />
          <Route path="/events/new" element={<EventFormPage />} />
          <Route path="/events" element={<EventsPage />} />
          {/* 추후 메모/할일/일정 라우트 추가 */}
        </Routes>
        <NavigationBar />
      </div>
    </BrowserRouter>
  );
};

export default App;

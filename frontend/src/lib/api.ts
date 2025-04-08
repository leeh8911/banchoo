export interface Memo {
    id: number;
    content: string;
}

export interface Task {
    id: number;
    content: string;
    status?: 'TODO' | 'DOING' | 'DONE';
    due_date?: string;
}

export interface Event {
    id: number;
    content: string;
    start_date?: string;
    end_date?: string;
}

// MEMO API
export async function fetchMemos(): Promise<Memo[]> {
    const res = await fetch('/memos');
    return res.json();
}

export async function createMemo(content: string): Promise<Memo> {
    const res = await fetch('/memos', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ content }),
    });
    return res.json();
}

// TASK API
export async function fetchTasks(): Promise<Task[]> {
    const res = await fetch('/tasks');
    return res.json();
}

export async function createTask(task: Omit<Task, 'id'>): Promise<Task> {
    const res = await fetch('/tasks', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(task),
    });
    return res.json();
}

// EVENT API
export async function fetchEvents(): Promise<Event[]> {
    const res = await fetch('/events');
    return res.json();
}

export async function createEvent(event: Omit<Event, 'id'>): Promise<Event> {
    const res = await fetch('/events', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(event),
    });
    return res.json();
}

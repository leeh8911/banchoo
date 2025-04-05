export interface Note {
    id?: number;
    content: string;
    status?: "TODO" | "DOING" | "DONE";
    start_date?: string;
    end_date?: string;
    due_date?: string;
}

const BASE_URL = import.meta.env.VITE_BASE_URL;


// ✅ 메모
export async function createMemo(content: string): Promise<Note> {
    const res = await fetch(`${BASE_URL}/memos`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ content }),
    });
    return await res.json();
}

export async function getMemos(): Promise<Note[]> {
    const res = await fetch(`${BASE_URL}/memos`);
    return await res.json();
}

// ✅ 작업(Task)
export async function createTask(note: Partial<Note>): Promise<Note> {
    const res = await fetch(`${BASE_URL}/tasks`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(note),
    });
    return await res.json();
}

export async function getTasks(): Promise<Note[]> {
    const res = await fetch(`${BASE_URL}/tasks`);
    return await res.json();
}

// ✅ 이벤트(Event)
export async function createEvent(note: Partial<Note>): Promise<Note> {
    const res = await fetch(`${BASE_URL}/events`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(note),
    });
    return await res.json();
}

export async function getEvents(): Promise<Note[]> {
    const res = await fetch(`${BASE_URL}/events`);
    return await res.json();
}

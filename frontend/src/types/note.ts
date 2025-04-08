export interface Memo {
    id: number;
    content: string;
    created_at: string;
    updated_at: string;
}
export interface Task {
    id: number;
    content: string;
    status?: 'todo' | 'done';
    due_date?: string;
    created_at: string;
    updated_at: string;
}
export interface Event {
    id: number;
    content: string;
    start_date?: string;
    end_date?: string;
    created_at: string;
    updated_at: string;
}
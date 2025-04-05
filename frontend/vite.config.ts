import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [react()],
  define: {
    'process.env.BASE_URL': JSON.stringify(process.env.BASE_URL || 'http://localhost:8080')
  },
  server: {
    host: '0.0.0.0',
    port: 5173
  }
});

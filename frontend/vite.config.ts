import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';
import path from 'path';

export default defineConfig({
  plugins: [react()],
  define: {
    'process.env.BASE_URL': JSON.stringify(process.env.BASE_URL || 'http://localhost:5173')
  },
  server: {
    host: '0.0.0.0',
    port: 5173, // ✅ 프론트는 5173 고정
    proxy: {
      '/memos': {
        target: 'http://localhost:18080',
        changeOrigin: true,
      },
      '/tasks': {
        target: 'http://localhost:18080',
        changeOrigin: true,
      },
      '/events': {
        target: 'http://localhost:18080',
        changeOrigin: true,
      },
      '/notes': {
        target: 'http://localhost:18080',
        changeOrigin: true,
      }
    }
  },
  resolve: {
    alias: {
      '@': path.resolve(__dirname, './src'),
    },
  },
});

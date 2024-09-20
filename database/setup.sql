-- Step 1: Create the llmchat database
CREATE DATABASE llmchat;

-- Step 2: Connect to the llmchat database
\c llmchat;

-- Step 3: Create the conversation table
CREATE TABLE conversation (
    id SERIAL PRIMARY KEY,
    type VARCHAR(10) NOT NULL, -- 'user' or 'ai'
    content TEXT NOT NULL,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Step 4: Create a user and grant privileges (run in the main database, not inside `llmchat`)
-- Disconnect from the current database with `\q` if necessary, then:
CREATE USER youruser WITH PASSWORD 'yourpassword';
GRANT ALL PRIVILEGES ON DATABASE llmchat TO youruser;

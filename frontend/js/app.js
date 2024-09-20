document.addEventListener("DOMContentLoaded", () => {
  const sendButton = document.getElementById("send-button");
  const userInput = document.getElementById("user-input");
  const chatBox = document.getElementById("chat-box");
  const historyPanel = document.getElementById("history-panel");
  const updateSettingsButton = document.getElementById("update-settings");
  const saveButton = document.getElementById("save-conversation");
  const shareButton = document.getElementById("share-conversation");

  let temperature = 0.7;
  let maxTokens = 150;

  // Initialize WebSocket
  const socket = new WebSocket("ws://localhost:8080/ws");

  socket.onopen = () => {
    console.log("Connected to the server");
    fetchConversationHistory();
  };

  socket.onmessage = (event) => {
    const message = JSON.parse(event.data);
    displayMessage(message);
    saveToHistory(message);
  };

  socket.onerror = (error) => {
    console.error("WebSocket error:", error);
  };

  // Send message on button click
  sendButton.addEventListener("click", () => {
    sendMessage();
  });

  // Send message on Enter key
  userInput.addEventListener("keypress", (e) => {
    if (e.key === "Enter") {
      sendMessage();
    }
  });

  function sendMessage() {
    const messageText = userInput.value.trim();
    if (messageText === "") return;

    const message = { type: "user", content: messageText };
    displayMessage(message);
    socket.send(JSON.stringify(message));
    userInput.value = "";
  }

  function displayMessage(message) {
    const messageElement = document.createElement("div");
    messageElement.classList.add(
      message.type === "user" ? "user-message" : "ai-message"
    );
    messageElement.textContent = message.content;
    chatBox.appendChild(messageElement);
    chatBox.scrollTop = chatBox.scrollHeight;
  }

  function saveToHistory(message) {
    const historyItem = document.createElement("div");
    historyItem.textContent = `${message.type === "user" ? "You" : "AI"}: ${
      message.content
    }`;
    historyPanel.appendChild(historyItem);
  }

  function fetchConversationHistory() {
    fetch("/api/history")
      .then((response) => response.json())
      .then((data) => {
        data.history.forEach((message) => displayMessage(message));
      })
      .catch((error) => console.error("Error fetching history:", error));
  }

  // Update settings
  updateSettingsButton.addEventListener("click", () => {
    temperature = parseFloat(document.getElementById("temperature").value);
    maxTokens = parseInt(document.getElementById("max_tokens").value);

    const settings = { temperature, max_tokens: maxTokens };
    socket.send(JSON.stringify({ type: "settings", content: settings }));
  });

  // Save conversation
  saveButton.addEventListener("click", () => {
    fetch("/api/save-conversation", { method: "POST" })
      .then((response) => response.json())
      .then((data) => alert("Conversation saved successfully!"))
      .catch((error) => console.error("Error saving conversation:", error));
  });

  // Share conversation
  shareButton.addEventListener("click", () => {
    fetch("/api/share-conversation", { method: "POST" })
      .then((response) => response.json())
      .then((data) => {
        const shareableLink = `${window.location.origin}/share/${data.conversationId}`;
        navigator.clipboard
          .writeText(shareableLink)
          .then(() => alert("Shareable link copied to clipboard!"))
          .catch((err) => console.error("Error copying link:", err));
      })
      .catch((error) => console.error("Error sharing conversation:", error));
  });
});

const sidebar = document.getElementById("sidebar");
const toggleBtn = document.getElementById("toggleBtn");
const userInfo = document.getElementById("user-info");
const createTaskBtn = document.getElementById("create-task-btn");
const body = document.body;

// Alternar sidebar colapsada
toggleBtn.addEventListener("click", () => {
  sidebar.classList.toggle("collapsed");
});

// Simulação de usuário logado
const userLoggedIn = true; // Troque para false para testar "Entrar"

if (userLoggedIn) {
  const userName = "WOLFSBURG";
  const avatarUrl = "https://via.placeholder.com/40";

  userInfo.innerHTML = `
    <img src="${avatarUrl}" alt="Avatar" />
    <div class="user-text">
      <div>Bem-vindo</div>
      <strong>${userName}</strong>
    </div>
  `;
} else {
  userInfo.innerHTML = `
    <div class="user-text">
      <strong><a href="#" style="color: var(--text-color); text-decoration: underline;">Entrar</a></strong>
    </div>
  `;
}

// Criar nova tarefa (botão)
createTaskBtn.addEventListener("click", () => {
  alert("Abrir modal de criação de tarefa (em breve)");
});

// Botões que podem ficar ativos
const buttons = document.querySelectorAll(".menu a, .services div, .settings div");

buttons.forEach((btn) => {
  btn.addEventListener("click", (e) => {
    e.preventDefault();

    // Remove active de todos
    buttons.forEach(b => b.classList.remove("active"));

    // Adiciona active no clicado
    btn.classList.add("active");

    // Pega ação e executa (exemplo com alert)
    const action = btn.getAttribute("data-action");
    console.log(`Ação clicada: ${action}`);

    switch (action) {
      case "dashboard":
        alert("Abrir Painel");
        break;
      case "calendar":
        alert("Abrir Calendário");
        break;
      case "tasks":
        alert("Mostrar Minhas Tarefas");
        break;
      case "mail":
        alert("Abrir E-mail");
        break;
      case "sales":
        alert("Mostrar Vendas");
        break;
      case "notifications":
        alert("Mostrar Notificações");
        break;
      case "faces":
        alert("Abrir UI Faces");
        break;
      case "proton":
        alert("Abrir Proton");
        break;
      case "ravencoin":
        alert("Abrir Ravencoin");
        break;
      case "binance":
        alert("Abrir Binance");
        break;
      case "preferences":
        alert("Abrir Preferências");
        break;
      case "settings-notifications":
        alert("Abrir Configurações de Notificações");
        break;
      case "theme":
        toggleTheme();
        break;
      case "logout":
        alert("Logout");
        break;
      default:
        break;
    }
  });
});

// Função para alternar tema claro/escuro
function toggleTheme() {
  body.classList.toggle("dark-theme");
}

// --- Código básico para iniciar canvas e renderizar algo ---

const canvas = document.getElementById("render-canvas");
const ctx = canvas.getContext("2d");

// Ajusta o tamanho do canvas para preencher a área
function resizeCanvas() {
  canvas.width = canvas.clientWidth;
  canvas.height = canvas.clientHeight;
}
window.addEventListener("resize", resizeCanvas);
resizeCanvas();

// Exemplo simples: desenha um círculo vermelho no centro
function render() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  ctx.fillStyle = "red";
  ctx.beginPath();
  ctx.arc(canvas.width/2, canvas.height/2, 50, 0, Math.PI * 2);
  ctx.fill();

  requestAnimationFrame(render);
}

render();

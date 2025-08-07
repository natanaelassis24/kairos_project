document.getElementById("addSphereBtn").addEventListener("click", () => {
    const panel = document.getElementById("spherePanel");
    panel.classList.toggle("hidden");
});

document.getElementById("confirmAddSphere").addEventListener("click", () => {
    const massa = parseFloat(document.getElementById("massInput").value);
    const densidade = parseFloat(document.getElementById("densityInput").value);
    const raio = parseFloat(document.getElementById("radiusInput").value);
    const x = parseFloat(document.getElementById("posX").value);
    const y = parseFloat(document.getElementById("posY").value);
    const z = parseFloat(document.getElementById("posZ").value);

    const esfera = {
        massa,
        densidade,
        raio,
        posicao: { x, y, z }
    };

    // Envia para o motor C++ via WebView2
    if (window.chrome?.webview) {
        window.chrome.webview.postMessage(JSON.stringify({
            tipo: "nova_esfera",
            dados: esfera
        }));
    }

    // Fecha o painel após adicionar
    document.getElementById("spherePanel").classList.add("hidden");
});

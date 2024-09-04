var Module = {
    preRun: [],
    postRun: [],
    print: (...args) => {
        const text = args.join(" ");
        console.log(text);
    },
    canvas: document.getElementById("canvas"),
    setStatus: (text) => {
        const statusElement = document.getElementById("status");
        if (statusElement) {
            statusElement.innerHTML = text;
        }
    },
    monitorRunDependencies: (left) => {
        Module.setStatus(left ? `Carregando... (${left})` : "Pronto");
    },
};

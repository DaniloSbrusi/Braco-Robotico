#ifndef PAGINAS_H
#define PAGINAS_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="pt-br">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Braço Robótico</title>
  <link rel="icon" href="data:,">
  <!-- Bootstrap CSS -->
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css" rel="stylesheet">
</head>
<body class="bg-light">
  <div class="container py-5">
    <div class="text-center">
      <h1 class="display-4 text-primary">Página WEB do Braço Robótico</h1>
      <p class="lead text-muted">Aqui você poderá comandar e monitorar o equipamento!</p>
    </div>
    <div class="mt-4 text-center">
      <button class="btn btn-primary btn-lg">Comandar Braço</button>
      <button class="btn btn-secondary btn-lg">Monitorar</button>
    </div>
  </div>

  <!-- Bootstrap JS Bundle with Popper -->
  <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/js/bootstrap.bundle.min.js"></script>
</body>
</html>
)rawliteral";

#endif

<p align='center' >
  <img width="250px" loading="lazy" src = "https://github.com/Geral-cp-s/Sprint-Edge/assets/110639916/aa204473-bba7-4dc2-8db1-ea5744b8e9bc"/>
</p>
<h1 align="Center">Projeto "Inovação Azul" - Monitoramento da Saúde dos Oceanosb Descrição</h1>

# Índice
* [Descrição do Projeto](#descricao)
* [Funcionalidades](#funcionalidades)
* [Componentes utilizados](#componentes)
* [Exibição](#exibicao)
* [Vídeo](#video)
* [Código](#codigo)
* [Acesso ao simulador](#simulador)
* [Autores](#autores)


<h2 id="Descricao">Descrição do projeto</h2>
<p>Este projeto combina um medidor de tempo de voltas com um sensor de pista + carro monitoran temperatura, umidade, chuva, consumo de corrente, velocidade, e registro de data e hora, proporcionando uma visão abrangente dos dados relevantes para a Fórmula E.</p>

<h2 id="funcionalidades">Funcionalidades</h2>
  <h3>Medição de Voltas</h3>
  
 - `Funcionalidade 1` <strong>Cronômetro de Voltas:</strong> Mede o tempo de cada volta utilizando um botão para iniciar e parar a contagem;

 - `Funcionalidade 2` <strong>Melhor e Pior Volta:</strong> Registra e exibe a melhor e pior volta no Monitor Serial.

  <h3>Monitoramento Ambiental e do Veículo</h3>
  
 - `Funcionalidade 1` <strong>Medição Ambiental:</strong> Utiliza sensores para capturar dados de umidade, temperatura e chuva;

 - `Funcionalidade 2` <strong>Medição do Veículo:</strong> Coleta dados sobre a rotação por minuto (RPM), transformados em Km/h, e o consumo de corrente do carro.

 - `Funcionalidade 3` <strong>Registro de Dados:</strong> Utiliza um RTC (Real-Time Clock) para registrar data e hora.
   
 - `Funcionalidade 4` <strong>Alertas e Exibição:</strong> Exibe informações em um display LCD e aciona alertas visuais e sonoros baseados nas condições de chuva.

<h2 id="Componentes">Componentes Utilizados</h2>
<h3>Medição de Voltas</h3>
  <ul>
    <li>Arduino UNO;</li>
    <li>Botão: Para iniciar e parar o cronômetro de voltas.</li>
    <li>LCD I2C: Display para exibir informações.</li>
    <li>RTC DS3231: Para manter o registro de data e hora.</li>
  </ul>
  
  <h3>Monitoramento Ambiental e do Veículo</h3>
  <ul>
    <li>DHT22: Sensor de temperatura e umidade.</li>
    <li>RTC DS3231: Para manter o registro de data e hora.</li>
    <li>Sensor de Chuva (Potenciomatro): Para medir a quantidade de chuva.</li>
    <li>Potenciômetro: Para simular o consumo de corrente.</li>
    <li>Encoder Rotativo: Para medir RPM.</li>
    <li>LCD I2C: Display para exibir informações.</li>
    <li>LEDs: Indicadores visuais de status (verde, amarelo e vermelho).</li>
    <li>Buzzer: Para alertas sonoros.</li>
    <li>EEPROM: Para armazenamento de eventos.</li>
  </ul>

<h2 id="exibicao">Exibição no LCD</h2>
<p>O LCD exibe uma variedade de informações, incluindo:</p>
<ul>
  <li>Data e Hora: Atualizada em tempo real.
</li>
  <li>Dados da Pista: Temperatura, umidade e quantidade de chuva.
</li>
  <li>Dados do Carro: Corrente de bateria e velocidade em km/h.
</li>
  <li>Alertas de Chuva: Diferentes mensagens e emojis baseados na intensidade da chuva.
</li>
</ul>


<h2 id="video">Vídeo</h2>
https://www.youtube.com/watch?v=ZCt1VZnbsps

<h2 id="codigo">Codigo</h2>
<p>Faça o upload do código para o seu microcontrolador. O código inclui a inicialização dos sensores, leitura de dados e exibição no display LCD, além de lógica para acionar alertas baseados na quantidade de chuva detectada e medir o tempo de voltas.</p>
<h3>Sensor para a pista e carro:</h3>
https://github.com/Geral-cp-s/Sprint-Edge/blob/main/sensor-pista-e-carro.ino

<h3>Tempo de volta</h3>
https://github.com/Geral-cp-s/Sprint-Edge/blob/main/sensor-tempo-da-volta.ino

<h2 id="Simulador">Acesso ao simulador</h2>
<h3>Sensor para a pista e carro:</h3>
https://wokwi.com/projects/400339562089182209

<h3>Tempo de volta</h3>
https://wokwi.com/projects/400310566728933377

<h2 id="Autores">Autores</h2>

<div align="center">
  
| [<img loading="lazy" src="https://github.com/gvqsilva/CP2-Edge/assets/110639916/d022ed18-0057-4944-9e00-db796c6d2e45" width=115><br><sub>Gabriel Vasquez</sub>](https://github.com/gvqsilva)  |  [<img loading="lazy" src="https://github.com/gvqsilva/CP2-Web/assets/110639916/1eb7df1a-c0e8-4170-aabf-444cfb3c64f9" width=115><br><sub>Guilherme Araujo</sub>](https://github.com/guilhermearaujodec)  |  [<img loading="lazy" src="https://github.com/gvqsilva/CP2-Edge/assets/110639916/86514492-2b1e-4422-bdc0-0ec3c8be3dcc" width=115><br><sub>Augusto Douglas</sub>](https://github.com/gutomend)  |  [<img loading="lazy" src="https://github.com/gvqsilva/CP2-Edge/assets/110639916/4bb3084d-d1ff-4b49-ba37-96c8046f6e14" width=115><br><sub>Gustavo Oliveira</sub>](https://github.com/Gusta346) |
| :---: | :---: | :---: | :---: |

<ul>
  <li>Gabriel Vasquez - RM: 557056</li>
  <li>Guilherme Araujo - RM: 558926</li>
  <li>Gustavo Oliveira - RM: 559163</li>
  <li>Augusto Mendonça - RM: 558371</li>
</ul><br>

<p>Boas corridas e boa sorte na sua implementação! 🚗💨</p>
</div>


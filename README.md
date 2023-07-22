# Trabalho Final - Fundamentos de Sistemas Embarcados

| Nome | Matrícula |
| ------------- | ------------- |
| Adne Moretti Moreira  | 200013181  |
| Caio Vitor Carneiro de Oliveira | 200057227 |
| Gabriel Moretti de Souza  |  200018205 |

Neste repositório consta o código referente ao [trabalho final](https://gitlab.com/fse_fga/trabalhos-2023_1/trabalho-3-2023-1) da disciplina de Fundamentos de Sistemas Embarcados, que, em nosso caso, consiste em um sistema de gerenciamento e segurança de uma **sala de servidores**, utilizando 3 dispositivos **ESP32**, conexão **WIFI** e o protocolo **MQTT** para detectar a entrada e saída de indivíduos por meio de uma porta automática, detectar sons incomuns e também detectar falhas na temperatura e/ou umidade que possam afetar a sala de servidores de alguma forma, alertando assim por meio de um aviso sonoro e por um LED que piscará com uma cor de acordo com o motivo do alarme.

Para melhor organização de estados e economia de energia também foram utilizados, respectivamento, o **NVS Flash** e o modo **Low Power** com light sleep, garantindo assim o melhor funcionamento do trabalho.

# Hardware

Como citado anteriormente, o hardware para esse repositório contém 3 dispositivos ESP32, com suas diferentes entradas e saídas expostas nos 6 elementos abaixo:

### ESP 1 (referenciado no código como ESP0)

- Sensor hall magnético (GPIO 15)
- Sensor de temperatura e umidade DHT11 (GPIO 4)

### ESP 2 (referenciado no código como ESP1)

- Sensor de Toque (GPIO 18)
- Buzzer ativo / passivo (GPIO 2)

### ESP 3 (referenciado no código como ESP2)

- LED RGB (GPIOs 23, 22 e 21 para, respectivamente, saídas R, G e B)
- Sensor de Som (GPIO 19)

# Dependências

[Visual Studio Code](https://code.visualstudio.com/)

[Espressif](https://www.espressif.com/)

# Como rodar

Para rodar, primeiro é necessário definir as variáveis de ambiente no menuconfig, utilizando o terminal da ESP-IDF e utilizando o comando:

```bash
idf.py menuconfig                                                                                                                               
```

Após isso, pode-se fazer o build do projeto clicando, no menu inferior, em: 

**ESP-IDF: Build project**

E compilar o código clicando em

**ESP-IDF: Flash device**

Visualizando, portanto, o código em funcionamento clicando em:

**ESP-IDF: Monitor device**

Assim permitindo o acompanhamento dos acontecimentos do código em funcionamento. Lembrando que o processo por completo também pode ser realizado por meio do clique no botão:

**ESP-IDF: Build, Flash and Monitor**

# Dashboard

A dashboard foi criada com [ThingsBoard](https://thingsboard.io/) e pode ser acessada em Dashboards com nome *"Server Security (AdneCaioGabriel)"*. Segue uma imagem do dash:

![image](https://github.com/FGA-FSE/trabalho-final-2023-1-adnecaiogabriel/assets/64036847/bdbaab8e-7bce-409c-93e9-ee050d4fddc5)



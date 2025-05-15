#ifndef VL53L5CX_H // Proteção contra inclusões múltiplas
#define VL53L5CX_H

#include <SparkFun_VL53L5CX_Library.h> // Biblioteca para comunicação com o sensor VL53L5CX

/**
 * @class VL53L5CX
 * @brief Classe que encapsula a lógica de uso do sensor de distância VL53L5CX, permitindo configurar,
 *        inicializar e acessar medições em uma grade de alta resolução.
 */
class VL53L5CX {
private:
    // === Atributos de configuração ===

    int iSensorID;                        ///< ID identificador do sensor.
    int iImageResolution;                 ///< Resolução total da imagem (largura x largura).
    int iImageWidth;                      ///< Largura da imagem em pixels (ex: 4 ou 8).
    int iFrequency;                       ///< Frequência de coleta dos dados em Hz.
    int iSensorReset;                     ///< Pino de reset do sensor.
    int iSensorAddress;                   ///< Endereço I2C do sensor.

    SparkFun_VL53L5CX myImager;           ///< Objeto da biblioteca que gerencia o sensor.
    VL53L5CX_ResultsData measurementData; ///< Estrutura de dados que armazena as medições do sensor.

    int distanceHistory[5][64];           ///< Histórico das últimas 5 leituras de cada célula (para filtro mediano temporal).
    int iSamplesCollected = 0;            ///< Contador de coletas feitas até o momento.

    /**
     * @brief Define o ID do sensor.
     * @param iSensorID ID desejado.
     * @return true se definido com sucesso.
     */
    bool bSetSensorID(int iSensorID) {
        this->iSensorID = iSensorID;
        return true;
    }

    /**
     * @brief Define a largura e resolução da imagem.
     * @param iImageWidth Largura (ex: 4 ou 8).
     * @return true se definido com sucesso.
     */
    bool bSetImageDimensions(int iImageWidth) {
        this->iImageWidth = iImageWidth;
        this->iImageResolution = iImageWidth * iImageWidth;
        return true;
    }

    /**
     * @brief Define a frequência de coleta de dados.
     * @param iFrequency Frequência em Hz.
     * @return true se definido com sucesso.
     */
    bool bSetDataFrequency(int iFrequency){
        this->iFrequency = iFrequency;
        return true;
    }

    /**
     * @brief Define o pino de reset do sensor.
     * @param iSensorReset Número do pino.
     * @return true se definido com sucesso.
     */
    bool bSetReset(int iSensorReset){
        this->iSensorReset = iSensorReset;
        return true;
    }

    /**
     * @brief Define o endereço I2C do sensor.
     * @param iSensorAddress Endereço desejado.
     * @return true se definido com sucesso.
     */
    bool bSetAddress(int iSensorAddress){
        this->iSensorAddress = iSensorAddress;
        return true;
    }

public:
    /**
     * @brief Construtor padrão.
     */
    VL53L5CX() {}

    /**
     * @brief Destrutor da classe VL53L5CX.
     * 
     * Encerra o modo de medição contínua para liberar recursos do sensor.
     */
    ~VL53L5CX() {
        myImager.stopRanging();
        Serial.print("Sensor ID ");
        Serial.print(iSensorID);
        Serial.println(" desligado.");
    }

    /**
     * @brief Inicializa o sensor com os parâmetros fornecidos.
     * @param iSensorID ID do sensor.
     * @param iImageWidth Largura da grade de leitura (ex: 4 ou 8).
     * @param iFrequency Frequência de coleta (em Hz).
     * @param iSensorAddress Endereço I2C desejado.
     * @param iSensorReset Pino de reset do sensor.
     * @return true se a inicialização for bem-sucedida.
     */
    bool bInit(int iSensorID, int iImageWidth, int iFrequency, int iSensorAddress, int iSensorReset) {
        delay(2000); // Aguarda estabilização
        bSetSensorID(iSensorID);
        bSetImageDimensions(iImageWidth);
        bSetDataFrequency(iFrequency);
        bSetAddress(iSensorAddress);
        bSetReset(iSensorReset);

        Serial.print("Inicializando sensor ");
        Serial.println(iSensorID);

        if (!myImager.begin()) {
            Serial.println("Sensor não encontrado. Verifique a fiação ou canal TCA.");
            return false;
        }

        if (myImager.getAddress() != iSensorAddress) {
            Serial.print(F("Alterando endereço do sensor para: 0x"));
            Serial.println(iSensorAddress, HEX);

            if (!myImager.setAddress(iSensorAddress)) {
                Serial.println(F("Falha ao alterar endereço do sensor. Travando..."));
                while (1);
            }
        }

        Serial.print(F("Novo endereço do sensor: 0x"));
        Serial.println(myImager.getAddress(), HEX);

        myImager.setResolution(iImageResolution);
        myImager.setRangingFrequency(iFrequency);
        myImager.startRanging();

        Serial.print("Sensor ");
        Serial.print(iSensorID);
        Serial.println(" inicializado com sucesso.\n");

        return true;
    }

    /**
     * @brief Retorna a resolução total da imagem (número de células).
     * @return Resolução da imagem.
     */
    int iGetImageResolution() {
        return iImageResolution;
    }

    /**
     * @brief Retorna a largura da imagem (número de colunas).
     * @return Largura da imagem.
     */
    int iGetImageWidth() {
        return iImageWidth;
    }

    /**
     * @brief Retorna a frequência de coleta de dados configurada.
     * @return Frequência em Hz.
     */
    int iGetFrequency(){
        return myImager.getRangingFrequency();
    }

    /**
     * @brief Verifica se novos dados estão disponíveis para leitura.
     * @return true se dados estiverem prontos.
     */
    bool bIsDataReady() {
        return myImager.isDataReady();
    }

    /**
     * @brief Solicita os dados de medição mais recentes ao sensor.
     * @return true se os dados foram obtidos com sucesso.
     */
    bool bGetRaningData() {
        return myImager.getRangingData(&measurementData);
    }

    /**
     * @brief Retorna a distância (em mm) da célula na posição indicada.
     * @param iCoordenates Índice da célula (0 a resolução-1).
     * @return Distância em milímetros.
     */
    int iGetDataMM(int iCoordenates) {
        return measurementData.distance_mm[iCoordenates];
    }

    /**
     * @brief Imprime os dados de distância no formato de grade via Serial.
     * 
     * Utiliza a largura da imagem para organizar a impressão linha por linha.
     */
    void printDistanceGrid() {
        if (bGetRaningData()) {
            for (int y = 0; y <= iImageWidth * (iImageWidth - 1); y += iImageWidth) {
                for (int x = iImageWidth - 1; x >= 0; x--) {
                    Serial.print("\t");
                    Serial.print(iGetDataMM(x + y));
                }
                Serial.println();
            }
            Serial.println();
        } else {
            Serial.print("Esperando coletas suficientes ");
            Serial.print(iSamplesCollected);
            Serial.println("/5");
        }
    }
};

#endif

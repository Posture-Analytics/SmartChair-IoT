#ifndef VL53L4CD__H // Proteção contra inclusões múltiplas
#define VL53L4CD__H

#include "SparkFun_VL53L1X.h" // Biblioteca do sensor VL53L1X (ou VL53L4CD)

/**
 * @class VL53L4CD_
 * @brief Classe que encapsula o uso do sensor de distância VL53L1X (ou VL53L4CD).
 */
class VL53L4CD_ {
private:
    int iSensorID;                        ///< ID do sensor, útil para identificar múltiplos sensores.
    SFEVL53L1X distanceSensor;            ///< Objeto da biblioteca que representa o sensor de distância.

    /**
     * @brief Define o ID do sensor.
     * @param iSensorID ID numérico para identificação do sensor.
     * @return true sempre (padrão de retorno para chamadas internas).
     */
    bool bSetSensorID(int iSensorID) {
        this->iSensorID = iSensorID;
        return true;
    }

public:
    /**
     * @brief Construtor padrão.
     */
    VL53L4CD_() {}

    /**
     * @brief Destrutor da classe VL53L4CD_.
     * 
     * Encerra o modo de medição contínua para liberar recursos do sensor.
     */
    ~VL53L4CD_() {
        distanceSensor.stopRanging();
        Serial.print("Sensor ID ");
        Serial.print(iSensorID);
        Serial.println(" desligado.");
    }

    /**
     * @brief Inicializa o sensor de distância.
     * 
     * Configura o sensor com um ID específico, define o modo de operação (curta ou longa distância)
     * e configura o intervalo entre medições.
     * 
     * @param iSensorID ID para identificação do sensor.
     * @param bShortMode Define se o sensor opera no modo de curta distância (true) ou longa distância (false). Default: true.
     * @param iIntermeasurementPeriod Intervalo entre medições, em milissegundos. Default: 100ms.
     * @return true se a inicialização for bem-sucedida.
     */
    bool bInit(int iSensorID, bool bShortMode = true, int iIntermeasurementPeriod = 100) {
        bSetSensorID(iSensorID);

        // Tenta iniciar o sensor
        if (distanceSensor.begin() != 0)
        {
            Serial.println("Falha ao iniciar o sensor.");
            while (1); // Travamento para depuração em caso de falha
        }
        Serial.println("Sensor inicializado.");

        // Define o modo de medição (curta ou longa distância)
        if (bShortMode){
            distanceSensor.setDistanceModeShort();
        } else {
            distanceSensor.setDistanceModeLong();
        }

        // Define o intervalo entre medições
        distanceSensor.setIntermeasurementPeriod(iIntermeasurementPeriod);

        // Inicia as medições contínuas
        distanceSensor.startRanging();

        return true;
    }

    /**
     * @brief Retorna a distância medida pelo sensor em milímetros.
     * 
     * A função espera até que a medição esteja disponível, depois lê o valor e
     * limpa a interrupção pendente.
     * 
     * @return Distância medida em milímetros.
     */
    int iGetDistanceMM(){
        // Espera os dados ficarem prontos
        while (!distanceSensor.checkForDataReady()) {
            delay(1);
        }

        // Lê a distância atual
        int distance = distanceSensor.getDistance();

        // Limpa a interrupção pendente
        distanceSensor.clearInterrupt();

        return distance;
    }
};

#endif
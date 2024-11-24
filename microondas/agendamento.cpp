#include <xinu.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int hora, minuto, tempoAquecimento;
    char alimento[50];
} Agendamento;

Agendamento agendamento;
void agendar(int hora, int minuto, const char* alimento, int tempo) {
    int ticks_atual = clkticks();
    int hora_atual = (ticks_atual / 3600) % 24;
    int minuto_atual = (ticks_atual % 3600) / 60;
    if (hora < hora_atual || (hora == hora_atual && minuto <= minuto_atual)) {
        kprintf("O horario ja passou.\n");
        return;
    }
    agendamento.hora = hora;
    agendamento.minuto = minuto;
    strncpy(agendamento.alimento, alimento, sizeof(agendamento.alimento) - 1);
    agendamento.alimento[sizeof(agendamento.alimento) - 1] = '\0';
    agendamento.tempoAquecimento = tempo;
    kprintf("Agendamento: %s em %02d:%02d por %d segundos.\n", alimento, hora, minuto, tempo);
}
void verificarAgendamento() {
    while (1) {
        int ticks_atual = clkticks();
        int hora_atual = (ticks_atual / 3600) % 24;
        int minuto_atual = (ticks_atual % 3600) / 60;
        if (hora_atual == agendamento.hora && minuto_atual >= agendamento.minuto) {
            kprintf("Iniciando aquecimento de %s por %d segundos...\n", agendamento.alimento, agendamento.tempoAquecimento);
            sleep(agendamento.tempoAquecimento);
            kprintf("Aquecimento de %s concluido!\n", agendamento.alimento);
            break;
        }
        sleep(1);
    }
}
process main(void) {
    agendar(14, 30, "pipoca", 120);

    kprintf("Verificando agendamento...\n");
    verificarAgendamento();

    return 0;
}

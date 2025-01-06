#ifndef VAD_H
#define VAD_H

#include <QObject>
#include <QAudioFormat>
#include <QVector>

class VAD : public QObject
{
    Q_OBJECT

public:
    explicit VAD(QObject *parent = nullptr);
    void processAudio(const QByteArray &audioData, const QAudioFormat &format);

signals:
    void voiceDetected(bool detected);

private:
    double calculateEnergy(const QVector<double> &samples);
    QVector<double> convertToPCM(const QByteArray &audioData, const QAudioFormat &format);
    int silentFrameCount;
};

#endif // VAD_H

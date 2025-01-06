#include "vad.h"
#include <cmath>
#include <QDebug>

VAD::VAD(QObject *parent) : QObject(parent), silentFrameCount(0)
{
}

void VAD::processAudio(const QByteArray &audioData, const QAudioFormat &format)
{
    QVector<double> samples = convertToPCM(audioData, format);
    double energy = calculateEnergy(samples);

    //qDebug() << "Energy:" << energy; // 打印能量值

    // 简单的能量阈值检测
    if (energy > 1000000.0) { // 调整阈值
        silentFrameCount = 0;
        emit voiceDetected(true);
    } else {
        silentFrameCount++;
        if (silentFrameCount > 10) { // 连续 10 帧静音
            emit voiceDetected(false);
        }
    }
}

double VAD::calculateEnergy(const QVector<double> &samples)
{
    double energy = 0.0;
    for (double sample : samples) {
        energy += sample * sample;
    }
    return energy / samples.size();
}

QVector<double> VAD::convertToPCM(const QByteArray &audioData, const QAudioFormat &format)
{
    QVector<double> samples;
    const int16_t *data = reinterpret_cast<const int16_t*>(audioData.constData());
    int numSamples = audioData.size() / format.bytesPerFrame();

    for (int i = 0; i < numSamples; ++i) {
        samples.append(static_cast<double>(data[i]));
    }

    return samples;
}

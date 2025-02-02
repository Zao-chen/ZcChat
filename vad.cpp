#include "vad.h"
#include <QSettings>
#include <QApplication>


VAD::VAD(QObject *parent) : QObject(parent), silentFrameCount(0)
{
}

void VAD::processAudio(const QByteArray &audioData, const QAudioFormat &format)
{
    QVector<double> samples = convertToPCM(audioData, format);
    double energy = calculateEnergy(samples);
    QSettings *settings = new QSettings(qApp->applicationDirPath()+"/Setting.ini",QSettings::IniFormat);
    emit energy_to_main(energy);
    // 简单的能量阈值检测
    if (energy > settings->value("/speechInput/energy").toInt()) { // 调整阈值
        silentFrameCount = 0;
        emit voiceDetected(true);
    } else {
        silentFrameCount++;
        if (silentFrameCount > 15) { // 连续 15 帧静音
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

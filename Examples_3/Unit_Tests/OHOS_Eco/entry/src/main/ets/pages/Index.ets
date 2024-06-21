import { hilog } from '@kit.PerformanceAnalysisKit';
import testNapi from 'libentry.so';
import { BusinessError } from '@ohos.base';
@Entry
@Component
struct Index {
  //renderer
  @State message: string = '';

  private xComponentId = 'The-Forge';
  //初始化函数
  aboutToAppear(): void {
    let context = getContext(this);
    testNapi.CreateSystem(context.resourceManager,context.filesDir,context.tempDir);
  }
  //初始化函数
  aboutToDisappear(): void {
    testNapi.ReleaseSystem();
  }

  onTestClick(idx: number) {
    this.message = "点字";
  }

  //样式
  private open: Boolean = false;
  private testArray: Array<string> = ['测试1', '测试2', '测试3', '测试4'];
  @State translateDistance: number = 0;

  build() {
    Column() {
      Column() {

        XComponent({ id: this.xComponentId, type: 'surface', libraryname: 'entry' })
          .onLoad(() => {
            hilog.info(0x0000, 'vulkanTest_OS', 'load');
          })
          .onDestroy(() => {
            hilog.info(0x0000, 'vulkanTest_OS', 'Destroy');
          })
          .backgroundColor(Color.White)
          .id('xComponent')
          .key('tetrahedron')

        //// Display
        //Text(this.message)
        //  .fontSize(50)
        //  .fontWeight(FontWeight.Bold)
        //  .onClick(() => this.onTestClick(-1))
      }
      .justifyContent(FlexAlign.SpaceAround)
      .alignItems(HorizontalAlign.Center)
      .height('100%')
      .layoutWeight(1)
      .width('100%')
      .layoutWeight(1)
      .backgroundColor(Color.Gray)
      .borderRadius(24)


      Stack() {
        //测试调c++的按钮
        ForEach(this.testArray, (name: string, idx: number) => {
          Button(name, { stateEffect: true }).width('20%')
            .translate({
              x: 0,
              y: this.translateDistance == 0 ? '0%' : (this.translateDistance + (160 * (idx + 1))).toString(10) + '%'
            })
            .onClick(() => this.onTestClick(idx))
        })
        //动画的按钮
        Button('测试选项', { stateEffect: true })
          .width('20%')
          .onClick(() => {
            if (!this.open) {
              animateTo({
                duration: 80,
                onFinish: () => {
                  this.open = true;
                }
              }, () => {
                this.translateDistance = -160 * (this.testArray.length + 1);
              })
            }
            else {
              animateTo({
                duration: 80,
                onFinish: () => {
                  this.open = false;
                }
              }, () => {
                this.translateDistance = 0
              })
            }
          })
      }
      .width('100%')
      .markAnchor({ x: '100%', y: '100%' })
      .position({ x: '100%', y: '95%' })
      .alignContent(Alignment.BottomStart)
    }
  }
}
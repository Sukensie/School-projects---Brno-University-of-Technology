using System.Windows;
using System.Windows.Input;

namespace ICSProject.App.Controls
{
    public partial class DestinationText
    {
        public string Txt
        {
            get => (string)GetValue(TxtProperty);
            set => SetValue(TxtProperty, value); 
        }

        public string Time
        {
            get => (string)GetValue(TimeProperty);
            set => SetValue(TimeProperty, value);
        }

        public static readonly DependencyProperty TxtProperty = DependencyProperty.Register(
            nameof(Txt),
            typeof(string),
            typeof(DestinationText),
            new PropertyMetadata(default(string)));

        public static readonly DependencyProperty TimeProperty = DependencyProperty.Register(
            nameof(Time),
            typeof(string),
            typeof(DestinationText),
            new PropertyMetadata(default(string)));

        public DestinationText()
        {
            InitializeComponent();
        }
    }
}

using System.Windows;

namespace ICSProject.App.Controls
{
    public partial class Heading
    {
        public string Txt
        {
            get => (string)GetValue(TxtProperty);
            set => SetValue(TxtProperty, value); 
        }

        public static readonly DependencyProperty TxtProperty = DependencyProperty.Register(
            nameof(Txt),
            typeof(string),
            typeof(Heading),
            new PropertyMetadata(default(string)));

        public Heading()
        {
            InitializeComponent();
        }
    }
}
